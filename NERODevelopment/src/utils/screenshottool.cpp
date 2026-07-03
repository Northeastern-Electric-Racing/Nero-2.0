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
  m_triggerPath = qEnvironmentVariable("NERO_SCREENSHOT_WATCH");
  if (!m_triggerPath.isEmpty()) {
    if (!QFile::exists(m_triggerPath))
      QFile(m_triggerPath).open(QIODevice::WriteOnly); // watcher needs a file
    if (!m_watcher.addPath(m_triggerPath))
      qWarning() << "NERO_SCREENSHOT_WATCH: cannot watch" << m_triggerPath;
    connect(&m_watcher, &QFileSystemWatcher::fileChanged, this,
            &ScreenshotTool::onTriggerFileChanged);
  }
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
  m_nav->jumpToPage(page);
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
  }
  if (quitAfter)
    QCoreApplication::quit();
}
