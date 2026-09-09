#ifndef SCREENSHOTTOOL_H
#define SCREENSHOTTOOL_H

#include <QFileSystemWatcher>
#include <QList>
#include <QObject>
#include <QString>

class QQmlApplicationEngine;
class NavigationController;

/**
 * @brief The ScreenshotTool class
 * Dev screenshot harness, inert unless its env vars are set
 * One-shot capture via NERO_SCREENSHOT and NERO_SCREENSHOT_OUT
 * On-demand capture enabled by NERO_SCREENSHOT_WATCH, trigger file path
 * from NERO_SCREENSHOT_WATCH_PATH (defaults to /tmp/nero-shot)
 */
class ScreenshotTool : public QObject {
  Q_OBJECT

public:
  ScreenshotTool(QQmlApplicationEngine *engine, NavigationController *nav,
                 QObject *parent = nullptr);

private:
  // One capture: the page to navigate to, where to save it, and the id the
  // caller wants echoed back (blank in one-shot mode, which has no waiter).
  struct Request {
    QString page;
    QString out;
    QString id;
    bool quitAfter = false;
  };

  void setupWatch();
  void onTriggerFileChanged();
  // Navigate to a top-level page and save it as a PNG
  void startCapture(const Request &req);
  void grabAndSave(const Request &req);
  void finishCapture(bool ok, const QString &detail, const Request &req);
  void signalDone(bool ok, const QString &detail, const QString &requestId);

  QQmlApplicationEngine *m_engine;
  NavigationController *m_nav;
  QFileSystemWatcher m_watcher;
  QString m_triggerPath;
  // Captures run one at a time. A second request arriving during the first
  // one's settle delay would navigate the window out from under that grab and
  // have its reply stamped with the wrong id, so it queues here instead.
  bool m_capturing = false;
  QList<Request> m_pending;
};

#endif // SCREENSHOTTOOL_H
