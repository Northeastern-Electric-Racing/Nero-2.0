/**
 * doomcontroller.cpp
 *
 * DOOM integration for the NERO dashboard.
 * Bridges the doomgeneric C engine with Qt's C++ framework.
 *
 * The doomgeneric platform callbacks (DG_DrawFrame, DG_GetKey, etc.)
 * are implemented directly in this file via extern "C" — no separate
 * .c file needed.
 */

#include "doomcontroller.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

/* ============================================================
 * doomgeneric C headers
 * ============================================================ */
extern "C" {
#include "doomgeneric.h"
#include "doomkeys.h"
}

/* ============================================================
 * Platform bridge — static state for C callbacks
 * ============================================================ */
static DoomWorker *g_doomWorker = nullptr;

static void platform_frame_callback(uint32_t *framebuffer, int width, int height);
static int platform_getkey_callback(unsigned char *pressed, unsigned char *doomKey);

/* ============================================================
 * doomgeneric platform callbacks (extern "C")
 *
 * These are the functions doomgeneric calls at runtime.
 * This replaces the need for a separate doom_platform.c file.
 * ============================================================ */
extern "C" {

void DG_Init(void)
{
    printf("[DOOM] Platform initialized (NERO Qt backend)\n");
}

void DG_DrawFrame(void)
{
    platform_frame_callback(DG_ScreenBuffer, DOOMGENERIC_RESX, DOOMGENERIC_RESY);
}

void DG_SleepMs(uint32_t ms)
{
    QThread::msleep(ms);
}

uint32_t DG_GetTicksMs(void)
{
    static QElapsedTimer timer;
    static bool started = false;
    if (!started) {
        timer.start();
        started = true;
    }
    return static_cast<uint32_t>(timer.elapsed());
}

int DG_GetKey(int *pressed, unsigned char *doom_key)
{
    unsigned char p = 0;
    unsigned char k = 0;
    int result = platform_getkey_callback(&p, &k);
    if (result) {
        *pressed = static_cast<int>(p);
        *doom_key = k;
        return 1;
    }
    return 0;
}

void DG_SetWindowTitle(const char *title)
{
    printf("[DOOM] Title: %s\n", title);
}

}

/* ============================================================
 * Platform callback implementations
 * ============================================================ */

static void platform_frame_callback(uint32_t *framebuffer, int width, int height)
{
    if (!g_doomWorker) return;

    QImage frame(
        reinterpret_cast<const uchar *>(framebuffer),
        width,
        height,
        width * static_cast<int>(sizeof(uint32_t)),
        QImage::Format_RGB32
        );

    emit g_doomWorker->frameReady(frame.copy());
}

static int platform_getkey_callback(unsigned char *pressed, unsigned char *doomKey)
{
    if (!g_doomWorker) return 0;
    return g_doomWorker->dequeueKey(pressed, doomKey);
}

/* ============================================================
 * DoomImageProvider
 * ============================================================ */

DoomImageProvider::DoomImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
    , m_currentFrame(320, 200, QImage::Format_RGB32)
{
    m_currentFrame.fill(Qt::black);
}

QImage DoomImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(id)
    QMutexLocker lock(&m_mutex);

    if (size)
        *size = m_currentFrame.size();

    if (requestedSize.isValid() && requestedSize != m_currentFrame.size())
        return m_currentFrame.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    return m_currentFrame;
}

void DoomImageProvider::updateFrame(const QImage &frame)
{
    QMutexLocker lock(&m_mutex);
    m_currentFrame = frame;
}

/* ============================================================
 * DoomWorker — Runs the game loop on a dedicated thread
 * ============================================================ */

DoomWorker::DoomWorker(const QString &wadPath, QObject *parent)
    : QObject(parent)
    , m_wadPath(wadPath)
    , m_running(false)
{
}

DoomWorker::~DoomWorker()
{
    stop();
}

void DoomWorker::enqueueKey(const DoomKeyEvent &event)
{
    QMutexLocker lock(&m_keyMutex);
    if (m_keyQueue.size() < 64) {
        m_keyQueue.enqueue(event);
    }
}

int DoomWorker::dequeueKey(unsigned char *pressed, unsigned char *doomKey)
{
    QMutexLocker lock(&m_keyMutex);
    if (m_keyQueue.isEmpty())
        return 0;

    DoomKeyEvent event = m_keyQueue.dequeue();
    *pressed = event.pressed;
    *doomKey = event.doomKey;
    return 1;
}

void DoomWorker::start()
{
    if (m_running) return;

    g_doomWorker = this;

    QByteArray wadPathUtf8 = m_wadPath.toUtf8();

    char *argv[] = {
        const_cast<char *>("nero_doom"),
        const_cast<char *>("-iwad"),
        wadPathUtf8.data(),
        nullptr
    };
    int argc = 3;

    qInfo() << "[DOOM] Starting engine with WAD:" << m_wadPath;

    doomgeneric_Create(argc, argv);

    m_running = true;
    emit started();

    qInfo() << "[DOOM] Engine running, entering game loop";

    while (m_running) {
        doomgeneric_Tick();
    }

    qInfo() << "[DOOM] Game loop exited";
    g_doomWorker = nullptr;
    emit stopped();
}

void DoomWorker::stop()
{
    m_running = false;
}

/* ============================================================
 * DoomController — Main QML-facing controller
 * ============================================================ */

DoomController::DoomController(Model *model, QObject *parent)
    : QObject(parent)
    , m_model(model)
    , m_worker(nullptr)
    , m_gameThread(nullptr)
    , m_imageProvider(nullptr)
    , m_running(false)
    , m_frameCounter(0)
    , m_statusText("Press ENTER to start DOOM")
{
    QStringList wadSearchPaths = {
        QCoreApplication::applicationDirPath() + "/DOOM1.WAD",
        QCoreApplication::applicationDirPath() + "/doom1.wad",
        QDir::currentPath() + "/DOOM1.WAD",
        QDir::currentPath() + "/doom1.wad",
        "/opt/nero/DOOM1.WAD",
        "/usr/share/doom/DOOM1.WAD",
    };

    for (const QString &path : wadSearchPaths) {
        if (QFile::exists(path)) {
            m_wadPath = path;
            qInfo() << "[DOOM] Found WAD file:" << path;
            break;
        }
    }

    if (m_wadPath.isEmpty()) {
        qWarning() << "[DOOM] WAD file not found! Searched:" << wadSearchPaths;
        m_statusText = "DOOM1.WAD not found!";
    }
}

DoomController::~DoomController()
{
    stopGame();
}

DoomImageProvider *DoomController::createImageProvider()
{
    m_imageProvider = new DoomImageProvider();
    return m_imageProvider;
}

void DoomController::startGame()
{
    if (m_running) return;

    if (m_wadPath.isEmpty()) {
        m_statusText = "Cannot start: DOOM1.WAD not found";
        emit statusTextChanged();
        return;
    }

    qInfo() << "[DOOM] Starting game...";
    m_statusText = "Loading DOOM...";
    emit statusTextChanged();

    m_gameThread = new QThread(this);
    m_worker = new DoomWorker(m_wadPath);
    m_worker->moveToThread(m_gameThread);

    connect(m_gameThread, &QThread::started, m_worker, &DoomWorker::start);
    connect(m_worker, &DoomWorker::frameReady, this, &DoomController::onFrameReady, Qt::QueuedConnection);
    connect(m_worker, &DoomWorker::started, this, &DoomController::onWorkerStarted, Qt::QueuedConnection);
    connect(m_worker, &DoomWorker::stopped, this, &DoomController::onWorkerStopped, Qt::QueuedConnection);

    connect(m_worker, &DoomWorker::stopped, m_gameThread, &QThread::quit);
    connect(m_gameThread, &QThread::finished, m_worker, &QObject::deleteLater);

    m_gameThread->start();
}

void DoomController::stopGame()
{
    if (!m_running || !m_worker) return;

    qInfo() << "[DOOM] Stopping game...";
    m_worker->stop();

    if (m_gameThread) {
        m_gameThread->quit();
        m_gameThread->wait(3000);
        if (m_gameThread->isRunning()) {
            qWarning() << "[DOOM] Force terminating game thread";
            m_gameThread->terminate();
            m_gameThread->wait(1000);
        }
    }

    m_running = false;
    m_statusText = "Press ENTER to start DOOM";
    emit runningChanged();
    emit statusTextChanged();
}

void DoomController::sendKey(int doomKeyCode, bool pressed)
{
    if (!m_worker || !m_running) return;

    DoomKeyEvent event;
    event.pressed = pressed ? 1 : 0;
    event.doomKey = static_cast<unsigned char>(doomKeyCode);
    m_worker->enqueueKey(event);
}

void DoomController::onNeroButton(const QString &buttonName, bool pressed)
{
    unsigned char doomKey = mapNeroButtonToDoomKey(buttonName);
    if (doomKey == 0) return;

    if (!m_running && pressed && buttonName == "Enter") {
        startGame();
        return;
    }

    sendKey(doomKey, pressed);
}

void DoomController::onFrameReady(const QImage &frame)
{
    if (m_imageProvider) {
        m_imageProvider->updateFrame(frame);
    }
    m_frameCounter++;
    emit frameCounterChanged();
}

void DoomController::onWorkerStarted()
{
    m_running = true;
    m_statusText = "DOOM is running";
    emit runningChanged();
    emit statusTextChanged();
}

void DoomController::onWorkerStopped()
{
    m_running = false;
    m_statusText = "Press ENTER to start DOOM";
    emit runningChanged();
    emit statusTextChanged();
}

unsigned char DoomController::mapNeroButtonToDoomKey(const QString &buttonName)
{
    if (buttonName == "Forward")    return KEY_UPARROW;
    if (buttonName == "Backward")   return KEY_DOWNARROW;
    if (buttonName == "Left")       return KEY_LEFTARROW;
    if (buttonName == "Right")      return KEY_RIGHTARROW;
    if (buttonName == "Enter")      return KEY_USE;
    if (buttonName == "Up")         return KEY_FIRE;
    if (buttonName == "Down")       return KEY_TAB;
    return 0;
}
