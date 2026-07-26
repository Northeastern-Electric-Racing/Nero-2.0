#include "screenshottool.h"

#include "../controllers/navigationcontroller.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
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
  const QStringList request =
      QTextStream(&f).readAll().trimmed().split(' ', Qt::SkipEmptyParts);
  f.close();
  if (request.isEmpty()) // the truncation below re-fires into this branch
    return;
  f.open(QIODevice::WriteOnly); // truncate so the same page can re-fire

  const QString page = request.first();
  const QString out =
      request.size() > 1
          ? request.at(1)
          : QFileInfo(m_triggerPath).dir().filePath(page.toLower() + ".png");
  capture(page, out, /*quitAfter=*/false);
}

void ScreenshotTool::capture(const QString &page, const QString &out,
                             bool quitAfter) {
  if (!m_nav->jumpToPage(page)) {
    qWarning() << "NERO_SCREENSHOT: unknown page" << page;
    if (quitAfter)
      QCoreApplication::quit();
    return;
  }
  QTimer::singleShot(500, this, [this, out, quitAfter]() { // let it render
    grabAndSave(out, quitAfter);
  });
}

void ScreenshotTool::grabAndSave(const QString &out, bool quitAfter) {
  const QList<QObject *> roots = m_engine->rootObjects();
  if (auto *w = roots.isEmpty() ? nullptr
                                : qobject_cast<QQuickWindow *>(roots.first())) {
    const bool ok = w->grabWindow().save(out);
    qInfo() << "NERO_SCREENSHOT:" << (ok ? "saved" : "FAILED") << out;
  } else {
    qWarning() << "NERO_SCREENSHOT: no root window to grab";
  }
  if (quitAfter)
    QCoreApplication::quit();
}
