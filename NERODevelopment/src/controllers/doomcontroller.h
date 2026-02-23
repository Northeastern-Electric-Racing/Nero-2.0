#ifndef DOOMCONTROLLER_H
#define DOOMCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <QQueue>
#include <QQuickImageProvider>
#include <QElapsedTimer>

class Model;

struct DoomKeyEvent {
    unsigned char pressed;
    unsigned char doomKey;
};

/**
 * @brief Image provider that serves the current DOOM frame to QML
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
 * @brief Worker that runs the DOOM game loop on a separate thread
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

private slots:
    void onFrameReady(const QImage &frame);
    void onWorkerStarted();
    void onWorkerStopped();

private:
    unsigned char mapNeroButtonToDoomKey(const QString &buttonName);

    Model *m_model;
    DoomWorker *m_worker;
    QThread *m_gameThread;
    DoomImageProvider *m_imageProvider;

    bool m_running;
    int m_frameCounter;
    QString m_statusText;
    QString m_wadPath;
};

#endif // DOOMCONTROLLER_H
