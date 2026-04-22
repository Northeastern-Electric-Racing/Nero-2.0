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
 *   - DoomWorker: Runs the DOOM game loop on a dedicated QThread
 *   - DoomController: QML-facing controller (same pattern as SnakeController)
 *
 * Button input:
 *   Unlike FlappyBird/Snake which only need press events, DOOM requires both
 *   press AND release (hold forward to walk, release to stop). The existing
 *   ButtonController pattern is consume-on-read (fire-and-forget presses),
 *   so DoomController connects directly to Model::onCurrentDataChange and
 *   reads the raw button value from "Wheel/Buttons/button_id" to do edge
 *   detection — tracking transitions to generate press/release pairs.
 *
 * Thread lifecycle:
 *   startGame() creates a QThread + DoomWorker, moves worker to thread, starts it.
 *   stopGame() sets m_running=false → game loop exits → thread quits → worker deleted.
 *   The thread is guaranteed to be stopped when:
 *     - User presses Esc (DoomView.qml calls stopGame + goHome)
 *     - User navigates away (DoomView.qml onIsFocusedChanged calls stopGame)
 *     - DoomView.qml is destroyed (Component.onDestruction calls stopGame)
 *     - DoomController is destroyed (destructor calls stopGame)
 */

#include <QObject>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <QQueue>
#include <QQuickImageProvider>
#include <QElapsedTimer>
#include <QTimer>

#include "../models/model.h"

struct DoomKeyEvent {
    unsigned char pressed;
    unsigned char doomKey;
};

/**
 * @brief Image provider that serves the current DOOM frame to QML.
 *
 * QML references frames via: Image { source: "image://doom/frame?" + frameCounter }
 * The changing query param forces QML to re-request each new frame.
 */
class DoomImageProvider : public QQuickImageProvider
{
public:
    DoomImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override;
    void updateFrame(const QImage &frame);

private:
    QImage m_currentFrame;
    QMutex m_mutex;
};

/**
 * @brief Worker that runs the DOOM game loop on a separate thread.
 *
 * Created by DoomController::startGame(), destroyed automatically when the
 * thread finishes (via QThread::finished → deleteLater connection).
 * The game loop in start() blocks until stop() sets m_running to false.
 */
class DoomWorker : public QObject
{
    Q_OBJECT

public:
    explicit DoomWorker(const QString &wadPath, QObject *parent = nullptr);
    ~DoomWorker();

    void enqueueKey(const DoomKeyEvent &event);
    int dequeueKey(unsigned char *pressed, unsigned char *doomKey);

public slots:
    void start();
    void stop();

signals:
    void frameReady(const QImage &frame);
    void started();
    void stopped();

private:
    QString m_wadPath;
    volatile bool m_running;
    QQueue<DoomKeyEvent> m_keyQueue;
    QMutex m_keyMutex;
};

/**
 * @brief Main controller for DOOM in the NERO dashboard.
 *        Same pattern as FlappyBirdController and SnakeController.
 */
class DoomController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool running READ running NOTIFY runningChanged)
    Q_PROPERTY(int frameCounter READ frameCounter NOTIFY frameCounterChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)

public:
    explicit DoomController(Model *model, QObject *parent = nullptr);
    ~DoomController();

    bool running() const { return m_running; }
    int frameCounter() const { return m_frameCounter; }
    QString statusText() const { return m_statusText; }

    DoomImageProvider *createImageProvider();

    Q_INVOKABLE void startGame();
    Q_INVOKABLE void stopGame();
    Q_INVOKABLE void sendKey(int doomKeyCode, bool pressed);
    Q_INVOKABLE void onNeroButton(const QString &buttonName, bool pressed);

signals:
    void runningChanged();
    void frameCounterChanged();
    void statusTextChanged();
    void escapeRequested();

private slots:
    void onFrameReady(const QImage &frame);
    void onWorkerStarted();
    void onWorkerStopped();

    /**
     * @brief Called on every Model::onCurrentDataChange.
     *
     * Reads the raw value from "Wheel/Buttons/button_id" and compares
     * against m_lastButtonValue to detect edges (press/release).
     * Only processes buttons when DOOM is actively running.
     */
    void onDataChanged();

private:
    unsigned char mapNeroButtonToDoomKey(const QString &buttonName);
    unsigned char mapButtonValueToDoomKey(int value);
    void releaseCurrentButton();
    void pressButton(int value);

    /** Auto-releases the held key after a short delay */
    void autoRelease();

    Model *m_model;
    DoomWorker *m_worker;
    QThread *m_gameThread;
    DoomImageProvider *m_imageProvider;

    bool m_running;
    int m_frameCounter;
    QString m_statusText;
    QString m_wadPath;

    // Edge detection for hardware buttons.
    // Tracks the last raw value from "Wheel/Buttons/button_id".
    int m_lastButtonValue;

    // Auto-release timer: sends keyup after a brief delay so DOOM
    // registers a tap instead of an infinite hold. Needed because
    // the wheel hardware does not send release events to MQTT.
    QTimer *m_releaseTimer;
};

#endif // DOOMCONTROLLER_H
