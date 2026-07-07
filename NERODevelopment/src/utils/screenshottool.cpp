#include "screenshottool.h"

#include "../controllers/navigationcontroller.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QRegularExpression>
#include <QTextStream>
#include <QTimer>

ScreenshotTool::ScreenshotTool(QQmlApplicationEngine *engine,
                               NavigationController *nav, QObject *parent)
    : QObject(parent), m_engine(engine), m_nav(nav) {
  // Capture the named page once, then quit
  if (qEnvironmentVariableIsSet("NERO_SCREENSHOT")) {
    const QString page = qEnvironmentVariable("NERO_SCREENSHOT");
    const QString out = qEnvironmentVariable("NERO_SCREENSHOT_OUT", "shot.png");
    QTimer::singleShot(2000, this, [this, page, out]() { // wait for data + UI
      capture(page, out, /*quitAfter=*/true);
    });
  }

  // Capture on demand each time the trigger file is written
  if (qEnvironmentVariableIsSet("NERO_SCREENSHOT_WATCH"))
    setupWatch();
}

void ScreenshotTool::setupWatch() {
  m_triggerPath =
      qEnvironmentVariable("NERO_SCREENSHOT_WATCH_PATH", "/tmp/nero-shot");

  // Fail loudly instead of doing nothing when the path is unusable
  const QFileInfo info(m_triggerPath);
  if (info.isDir()) {
    qWarning() << "NERO_SCREENSHOT_WATCH_PATH is a directory, need a file path"
               << m_triggerPath;
    return;
  }
  if (!info.dir().exists()) {
    qWarning() << "NERO_SCREENSHOT_WATCH_PATH directory does not exist"
               << info.dir().absolutePath();
    return;
  }
  if (!QFile::exists(m_triggerPath) &&
      !QFile(m_triggerPath)
           .open(QIODevice::WriteOnly)) { // watcher needs a file
    qWarning() << "NERO_SCREENSHOT_WATCH_PATH cannot create trigger file"
               << m_triggerPath;
    return;
  }
  if (!m_watcher.addPath(m_triggerPath)) {
    qWarning() << "NERO_SCREENSHOT_WATCH_PATH cannot watch" << m_triggerPath;
    return;
  }
  connect(&m_watcher, &QFileSystemWatcher::fileChanged, this,
          &ScreenshotTool::onTriggerFileChanged);
  qInfo() << "NERO_SCREENSHOT_WATCH: watching" << m_triggerPath;
}

void ScreenshotTool::onTriggerFileChanged() {
  // Re-arm because editors replace the file on save and drop the watch
  if (!m_watcher.files().contains(m_triggerPath))
    m_watcher.addPath(m_triggerPath);

  QFile f(m_triggerPath);
  if (!f.open(QIODevice::ReadOnly))
    return;
  // QTextStream detects BOMs so Windows PowerShell writes parse too
  const QString request = QTextStream(&f).readAll().trimmed();
  f.close();
  if (request.isEmpty()) // the truncation below re-fires into this branch
    return;
  f.open(QIODevice::WriteOnly); // truncate so the same page can re-fire

  // Format is "PAGE [OUT]". Split on the last whitespace, but only when the
  // trailing token looks like a path, so multi-word labels ("PIT - DRIVE")
  // stay intact while an optional output path is still honored.
  QString page = request;
  QString out;
  const int lastWs = request.lastIndexOf(QRegularExpression("\\s"));
  if (lastWs >= 0) {
    const QString tail = request.mid(lastWs + 1);
    if (tail.contains('/') || tail.endsWith(".png", Qt::CaseInsensitive)) {
      page = request.left(lastWs).trimmed();
      out = tail;
    }
  }
  if (out.isEmpty()) // default next to the trigger file
    out = QFileInfo(m_triggerPath).dir().filePath(page.toLower() + ".png");
  capture(page, out, /*quitAfter=*/false);
}

void ScreenshotTool::capture(const QString &page, const QString &out,
                             bool quitAfter) {
  if (!m_nav->jumpToPage(page)) {
    qWarning() << "NERO_SCREENSHOT: unknown page" << page;
    signalDone(false, page); // let a waiting caller learn it failed
    if (quitAfter)
      QCoreApplication::quit();
    return;
  }
  // Settle delay is tunable so bulk captures aren't stuck at 500ms
  const int delayMs = qEnvironmentVariableIntValue("NERO_SCREENSHOT_DELAY_MS");
  QTimer::singleShot(delayMs > 0 ? delayMs : 500, this,
                     [this, out, quitAfter]() { // let it render
                       grabAndSave(out, quitAfter);
                     });
}

void ScreenshotTool::grabAndSave(const QString &out, bool quitAfter) {
  const QList<QObject *> roots = m_engine->rootObjects();
  bool ok = false;
  if (auto *w = roots.isEmpty() ? nullptr
                                : qobject_cast<QQuickWindow *>(roots.first())) {
    ok = w->grabWindow().save(out);
    qInfo() << "NERO_SCREENSHOT:" << (ok ? "saved" : "FAILED") << out;
  } else {
    qWarning() << "NERO_SCREENSHOT: no root window to grab";
  }
  signalDone(ok, out);
  if (quitAfter)
    QCoreApplication::quit();
}

// Announce a finished capture so callers can await it instead of polling the
// output file (which races the write). Emits a parseable stdout line in both
// modes; in watch mode also drops a "<trigger>.done" sentinel for callers that
// don't capture stdout.
void ScreenshotTool::signalDone(bool ok, const QString &detail) {
  const QString status = ok ? QStringLiteral("ok") : QStringLiteral("fail");
  qInfo().noquote()
      << QStringLiteral("NERO_SHOT_DONE %1 \"%2\"").arg(status, detail);

  if (m_triggerPath.isEmpty())
    return;
  QFile done(m_triggerPath + ".done");
  if (done.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
    QTextStream(&done) << status << " \"" << detail << "\"\n";
    done.close();
  }
}
