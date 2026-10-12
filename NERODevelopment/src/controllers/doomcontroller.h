#ifndef DOOMCONTROLLER_H
#define DOOMCONTROLLER_H

/**
 * DOOM integration for the NERO dashboard.
 *
 * Uses the doomgeneric engine (https://github.com/ozkl/doomgeneric), a minimal
 * portable DOOM source port designed for embedding. The engine code is pulled
 * in unmodified via CMake FetchContent — do not edit the engine .c files.
 *
 * This header defines the NERO-specific wrapper classes:
 *   - DoomImageProvider: Serves DOOM frames to QML via "image://doom/..."
 *   - DoomWorker: Ticks the engine on a dedicated QThread
 *   - DoomController: QML-facing controller
 *
 * Button input:
 *   DOOM needs both press and release (hold forward to walk, release to
 *   stop). The wheel only sends presses, so movement stays held until the next
 *   press and Enter auto-releases after a short delay.
 *
 * Engine lifetime:
 *   doomgeneric can only be created once per process, so the engine starts on
 *   the first resume() and lives until the app exits. Leaving the page only
 *   pauses it, and coming back continues the same game.
 */

#include <QImage>
#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QQuickImageProvider>
#include <QThread>
#include <QTimer>
#include <optional>

#include "buttons.h"

struct DoomKeyEvent {
  unsigned char pressed;
  unsigned char doomKey;
};

/**
 * @brief Image provider that serves the current DOOM frame to QML.
 *
 * QML references frames via: Image { source: "image://doom/frame?" +
 * frameCounter } The changing query param forces QML to re-request each new
 * frame.
 */
class DoomImageProvider : public QQuickImageProvider {
public:
  DoomImageProvider();
  QImage requestImage(const QString &id, QSize *size,
                      const QSize &requestedSize) override;
  void updateFrame(const QImage &frame);

private:
  QImage m_currentFrame;
  QMutex m_mutex;
};

/**
 * @brief Ticks from its thread's event loop, so pause and resume are queued
 * calls.
 */
class DoomWorker : public QObject {
  Q_OBJECT

public:
  explicit DoomWorker(const QString &wadPath);

  void enqueueKey(const DoomKeyEvent &event);
  int dequeueKey(unsigned char *pressed, unsigned char *doomKey);

public slots:
  void start();
  void setPaused(bool paused);

signals:
  void frameReady(const QImage &frame);

private:
  QString m_wadPath;
  QTimer *m_tickTimer = nullptr;
  QQueue<DoomKeyEvent> m_keyQueue;
  QMutex m_keyMutex;
};

class DoomController : public QObject {
  Q_OBJECT

  Q_PROPERTY(int frameCounter READ frameCounter NOTIFY frameCounterChanged)

public:
  explicit DoomController(QObject *parent = nullptr);
  ~DoomController();

  int frameCounter() const { return m_frameCounter; }

  DoomImageProvider *createImageProvider();

  Q_INVOKABLE void resume();
  Q_INVOKABLE void pause();
  Q_INVOKABLE void press(Buttons::Button button);

signals:
  void frameCounterChanged();

private slots:
  void onFrameReady(const QImage &frame);

private:
  void sendButton(Buttons::Button button, bool pressed);
  void releaseCurrentButton();

  DoomWorker *m_worker = nullptr;
  QThread m_gameThread;
  DoomImageProvider *m_imageProvider = nullptr;

  int m_frameCounter = 0;
  QString m_wadPath;

  std::optional<Buttons::Button> m_heldButton;

  QTimer m_releaseTimer;
};

#endif // DOOMCONTROLLER_H
