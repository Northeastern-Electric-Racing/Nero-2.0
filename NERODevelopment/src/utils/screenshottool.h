#ifndef SCREENSHOTTOOL_H
#define SCREENSHOTTOOL_H

#include <QFileSystemWatcher>
#include <QObject>

class QQmlApplicationEngine;
class NavigationController;

/**
 * @brief The ScreenshotTool class
 * Dev screenshot harness, inert unless its env vars are set
 * One-shot capture via NERO_SCREENSHOT and NERO_SCREENSHOT_OUT
 * On-demand capture via a NERO_SCREENSHOT_WATCH trigger file
 */
class ScreenshotTool : public QObject {
  Q_OBJECT

public:
  ScreenshotTool(QQmlApplicationEngine *engine, NavigationController *nav,
                 QObject *parent = nullptr);

  // Navigate to a top-level page and save it as a PNG
  void capture(const QString &page, const QString &out, bool quitAfter);

private:
  void onTriggerFileChanged();
  void grabAndSave(const QString &out, bool quitAfter);

  QQmlApplicationEngine *m_engine;
  NavigationController *m_nav;
  QFileSystemWatcher m_watcher;
  QString m_triggerPath;
};

#endif // SCREENSHOTTOOL_H
