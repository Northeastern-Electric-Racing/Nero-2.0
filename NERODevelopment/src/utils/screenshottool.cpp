#include "screenshottool.h"

#include "../controllers/navigationcontroller.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QImage>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QRegularExpression>
#include <QStringList>
#include <QTextStream>
#include <QTimer>

ScreenshotTool::ScreenshotTool(QQmlApplicationEngine *engine,
                               NavigationController *nav, QObject *parent)
    : QObject(parent), m_engine(engine), m_nav(nav) {
  // Capture the named page once, then quit
  if (qEnvironmentVariableIsSet("NERO_SCREENSHOT")) {
    Request req;
    req.page = qEnvironmentVariable("NERO_SCREENSHOT");
    req.out = qEnvironmentVariable("NERO_SCREENSHOT_OUT", "shot.png");
    req.quitAfter = true;
    QTimer::singleShot(2000, this, [this, req]() { // wait for data + UI
      startCapture(req);
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

  // A trigger file outlives the app that created it, so its presence alone says
  // nothing about liveness. Drop a pid next to it so a caller can spot a dead
  // app instead of waiting out its whole timeout. Cleared on a clean exit; a
  // killed app leaves it behind, which a caller reads as the dead pid it is.
  const QString pidPath = m_triggerPath + ".pid";
  QFile pid(pidPath);
  if (pid.open(QIODevice::WriteOnly | QIODevice::Truncate))
    pid.write(QByteArray::number(QCoreApplication::applicationPid()) + '\n');
  connect(qApp, &QCoreApplication::aboutToQuit, this,
          [pidPath]() { QFile::remove(pidPath); });

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
  if (!f.open(QIODevice::WriteOnly)) // truncate so the same page can re-fire
    qWarning() << "NERO_SCREENSHOT_WATCH: cannot truncate trigger, this request"
               << "may fire again" << m_triggerPath;

  // Preferred format is one field per line, "PAGE\nOUT\nID", so a label or an
  // output path containing spaces survives verbatim. OUT and ID are optional
  // and may be blank; ID is echoed back in the completion signal.
  QString page = request;
  QString out;
  QString id;
  if (request.contains('\n')) {
    const QStringList lines = request.split('\n');
    page = lines.value(0).trimmed();
    out = lines.value(1).trimmed();
    id = lines.value(2).trimmed();
  } else {
    // One-line "PAGE [OUT]": split on the last whitespace, but only when the
    // trailing token looks like a filename — a path separator ('/' or '\') or
    // a trailing extension (.png, .jpg, ...) — so multi-word labels
    // ("PIT - DRIVE") stay intact while an optional output path is honored.
    // An output path containing a space needs the line-per-field form above.
    static const QRegularExpression wsRe(QStringLiteral("\\s"));
    static const QRegularExpression fileExtRe(
        QStringLiteral("\\.[A-Za-z0-9]+$"));
    const int lastWs = request.lastIndexOf(wsRe);
    if (lastWs >= 0) {
      const QString tail = request.mid(lastWs + 1);
      if (tail.contains('/') || tail.contains('\\') ||
          fileExtRe.match(tail).hasMatch()) {
        page = request.left(lastWs).trimmed();
        out = tail;
      }
    }
  }
  if (out.isEmpty()) // default next to the trigger file
    out = QFileInfo(m_triggerPath).dir().filePath(page.toLower() + ".png");

  Request req;
  req.page = page;
  req.out = out;
  req.id = id;
  // Serialize: starting this one now would jump the window away mid-settle and
  // spoil the grab already pending, so wait for that one to report first.
  if (m_capturing) {
    m_pending.append(req);
    return;
  }
  startCapture(req);
}

void ScreenshotTool::startCapture(const Request &req) {
  m_capturing = true;
  if (!m_nav->jumpToPage(req.page)) {
    qWarning() << "NERO_SCREENSHOT: unknown page" << req.page;
    finishCapture(false, req.page, req); // let a waiting caller learn it failed
    return;
  }
  // Settle delay is tunable so bulk captures aren't stuck at 500ms; warn on a
  // set-but-bogus value instead of silently falling back, like setupWatch does.
  int delayMs = 500;
  if (qEnvironmentVariableIsSet("NERO_SCREENSHOT_DELAY_MS")) {
    bool ok = false;
    const int v = qEnvironmentVariableIntValue("NERO_SCREENSHOT_DELAY_MS", &ok);
    if (ok && v > 0)
      delayMs = v;
    else
      qWarning() << "NERO_SCREENSHOT_DELAY_MS ignored, want a positive integer:"
                 << qEnvironmentVariable("NERO_SCREENSHOT_DELAY_MS");
  }
  QTimer::singleShot(delayMs, this, [this, req]() { // let it render
    grabAndSave(req);
  });
}

// A grab where every pixel is identical means the scene never rendered — a
// common headless failure when the offscreen plugin has no way to draw the Qt
// Quick scene. Real NERO screens always have UI on them, so treat a uniform
// (or null) grab as a failed capture rather than silently saving a blank PNG.
static bool isBlankGrab(const QImage &image) {
  if (image.isNull() || image.width() <= 0 || image.height() <= 0)
    return true;
  // Escape hatch for the day a screen legitimately is one flat color — none is
  // today, every page draws the header plus content. A null grab still fails.
  if (qEnvironmentVariableIntValue("NERO_SCREENSHOT_ALLOW_UNIFORM") == 1)
    return false;
  const QImage img = image.convertToFormat(QImage::Format_RGB32);
  const QRgb first = *reinterpret_cast<const QRgb *>(img.constScanLine(0));
  for (int y = 0; y < img.height(); ++y) {
    const auto *line = reinterpret_cast<const QRgb *>(img.constScanLine(y));
    for (int x = 0; x < img.width(); ++x)
      if (line[x] != first)
        return false;
  }
  return true;
}

void ScreenshotTool::grabAndSave(const Request &req) {
  const QString &out = req.out;
  const QList<QObject *> roots = m_engine->rootObjects();
  bool ok = false;
  if (auto *w = roots.isEmpty() ? nullptr
                                : qobject_cast<QQuickWindow *>(roots.first())) {
    const QImage img = w->grabWindow();
    if (isBlankGrab(img)) {
      // Fail loudly: a blank grab usually means there was no render surface. On
      // headless Linux, launching with QT_QUICK_BACKEND=software fixes it.
      qWarning() << "NERO_SCREENSHOT: grab is blank, not saving" << out
                 << "- on headless Linux try QT_QUICK_BACKEND=software";
    } else {
      ok = img.save(out);
      qInfo() << "NERO_SCREENSHOT:" << (ok ? "saved" : "FAILED") << out;
    }
  } else {
    qWarning() << "NERO_SCREENSHOT: no root window to grab";
  }
  finishCapture(ok, out, req);
}

// Report a finished capture to its own caller, then let the next one start.
void ScreenshotTool::finishCapture(bool ok, const QString &detail,
                                   const Request &req) {
  signalDone(ok, detail, req.id);
  if (req.quitAfter) {
    QCoreApplication::quit();
    return;
  }
  m_capturing = false;
  if (!m_pending.isEmpty())
    startCapture(m_pending.takeFirst());
}

// Announce a finished capture so callers can await it instead of polling the
// output file (which races the write). Emits a parseable line to the app's log
// (qInfo, which Qt sends to stderr) in both modes; in watch mode also drops a
// "<trigger>.done" sentinel for callers that don't read the log.
void ScreenshotTool::signalDone(bool ok, const QString &detail,
                                const QString &requestId) {
  QString payload =
      QStringLiteral("%1 \"%2\"")
          .arg(ok ? QStringLiteral("ok") : QStringLiteral("fail"), detail);
  // Echo the id of the request this reply belongs to — the sentinel is shared,
  // so that is how a caller tells its own reply from another call's.
  if (!requestId.isEmpty())
    payload += QStringLiteral(" ") + requestId;
  qInfo().noquote() << "NERO_SHOT_DONE" << payload;

  if (m_triggerPath.isEmpty())
    return;
  QFile done(m_triggerPath + ".done");
  if (done.open(QIODevice::WriteOnly | QIODevice::Truncate))
    done.write((payload + QStringLiteral("\n")).toUtf8());
}
