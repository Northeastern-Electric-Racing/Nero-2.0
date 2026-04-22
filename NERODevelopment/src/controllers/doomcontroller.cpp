/**
 * doomcontroller.cpp
 *
 * DOOM integration for the NERO dashboard.
 * Bridges the doomgeneric C engine with Qt's C++ framework.
 *
 * The doomgeneric engine is a direct port of the original DOOM source code,
 * provided by: https://github.com/ozkl/doomgeneric
 *
 * The engine code itself (all .c files pulled in via FetchContent) is a direct
 * rip from that repository and is NOT custom NERO code. Do not modify the
 * engine sources — they should stay as-is from upstream.
 *
 * What IS custom NERO code is everything in this file: the platform callbacks
 * (DG_DrawFrame, DG_GetKey, etc.) implemented via extern "C", the
 * DoomImageProvider, DoomWorker, and DoomController classes. These bridge
 * the unmodified engine into Qt/QML for our dashboard.
 *
 * We also provide our own I_InitGraphics / I_FinishUpdate / I_SetPalette
 * (replacing i_video.c from the upstream repo) because the upstream version
 * opens /dev/fb0 directly, which conflicts with Qt's EGLFS on the Pi.
 * Our version renders to DG_ScreenBuffer in memory and lets Qt handle display.
 *
 * The doomgeneric platform callbacks are implemented directly in this file
 * via extern "C" blocks — no separate .c file is needed.
 */

#include "doomcontroller.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "../utils/data_type_names.h"

#include <cstdio>
#include <cstring>

/* ============================================================
 * doomgeneric C headers
 * Source: https://github.com/ozkl/doomgeneric/tree/master/doomgeneric
 * These headers are from the upstream repo and should not be modified.
 * ============================================================ */
extern "C" {
#include "doomgeneric.h"
#include "doomkeys.h"
#include "doomtype.h"
#include "d_event.h"
#include "z_zone.h"

extern int joybspeed;
extern void D_PostEvent(event_t *ev);
}

/* ============================================================
 * NERO button values from "Wheel/Buttons/button_id" MQTT topic.
 *
 * MQTT value = physical button number - 1 (0-indexed).
 * Physical buttons on the wheel PCB:
 *   Phys 1 (Escape)       → MQTT 0
 *   Phys 2 (Left)         → MQTT 1
 *   Phys 3 (Middle Left)  → MQTT 2
 *   Phys 4 (Up)           → MQTT 3
 *   Phys 5 (Down)         → MQTT 4
 *   Phys 6 (Enter)        → MQTT 5
 *   Phys 7 (Right)        → MQTT 6
 *   Phys 8 (Middle Right) → MQTT 7
 * ============================================================ */
static constexpr int BUTTON_VALUE_ESCAPE       = 0;  // physical 1 — escape
static constexpr int BUTTON_VALUE_LEFT         = 1;  // physical 2 — turn left
static constexpr int BUTTON_VALUE_MIDDLE_LEFT  = 2;  // physical 3 — also turn left
static constexpr int BUTTON_VALUE_UP           = 3;  // physical 4 — move forward
static constexpr int BUTTON_VALUE_DOWN         = 4;  // physical 5 — move backward
static constexpr int BUTTON_VALUE_ENTER        = 5;  // physical 6 — fire+use+menu
static constexpr int BUTTON_VALUE_RIGHT        = 6;  // physical 7 — turn right
static constexpr int BUTTON_VALUE_MIDDLE_RIGHT = 7;  // physical 8 — also turn right

/**
 * Returns true if the value corresponds to a known DOOM-mapped button.
 * Any value NOT in this set (e.g. -1, 10, 255) is treated as a release.
 */
static bool isKnownDoomButton(int value)
{
    return value >= 0 && value <= 7;
}

/**
 * Returns true if the button should be a brief tap (auto-release after timer).
 * These buttons send keydown then keyup after 100ms:
 *   - Middle Left (2): small left nudge
 *   - Middle Right (7): small right nudge
 *   - Enter (5): shoot once / use once / menu select
 *
 * All other buttons are TOGGLE — they stay held until a different
 * button is pressed:
 *   - Left (1): continuous turn left
 *   - Right (6): continuous turn right
 *   - Up (3): continuous move forward
 *   - Down (4): continuous move backward
 */
static bool isTapButton(int value)
{
    return value == BUTTON_VALUE_MIDDLE_LEFT
        || value == BUTTON_VALUE_MIDDLE_RIGHT
        || value == BUTTON_VALUE_ENTER;
}

/* ============================================================
 * Platform bridge — static state for C callbacks
 * ============================================================ */
static DoomWorker *g_doomWorker = nullptr;

static void platform_frame_callback(uint32_t *framebuffer, int width, int height);
static int platform_getkey_callback(unsigned char *pressed, unsigned char *doomKey);

/* ============================================================
 * Video subsystem state (replaces i_video.c)
 *
 * The upstream i_video.c opens /dev/fb0 and mmaps the Linux
 * framebuffer, which crashes on the Pi because Qt EGLFS already
 * owns the display. Our replacement renders entirely in memory:
 *
 *   1. DOOM renders to I_VideoBuffer (8-bit indexed color)
 *   2. I_FinishUpdate converts indexed → XRGB using s_palette
 *   3. Result goes into DG_ScreenBuffer (allocated by doomgeneric.c)
 *   4. DG_DrawFrame is called → Qt picks up the frame
 * ============================================================ */
static uint32_t s_palette[256];

/* ============================================================
 * Variables and functions that i_video.c normally defines.
 * These are referenced by the rest of the DOOM engine and must
 * be provided since we excluded i_video.c from the build.
 * ============================================================ */
extern "C" {

/* Video buffers — screens[0] is the primary 8-bit indexed buffer */
byte *I_VideoBuffer = NULL;
byte *screens[5] = { NULL, NULL, NULL, NULL, NULL };

/* Video settings referenced by the engine */
int screenvisible = 1;
int screensaver_mode = 0;
int usegamma = 0;
int usemouse = 0;
int mouse_acceleration = 2;
int mouse_threshold = 10;

} /* extern "C" */

/* ============================================================
 * doomgeneric platform callbacks (extern "C")
 *
 * These are the functions the doomgeneric engine calls at runtime.
 * The engine expects these symbols to exist at link time — without them
 * the build will fail with undefined references.
 *
 * This is the NERO-specific platform backend, equivalent to the
 * doomgeneric_sdl.c or doomgeneric_xlib.c files in the upstream repo.
 * We implement them here in C++ (via extern "C") to avoid needing a
 * separate .c file and to directly access Qt APIs.
 * ============================================================ */
extern "C" {

void DG_Init(void)
{
    fprintf(stderr, "[DOOM] Platform initialized (NERO Qt backend)\n");
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
    fprintf(stderr, "[DOOM] Title: %s\n", title);
}

/* ============================================================
 * Video functions (replacing i_video.c)
 *
 * The DOOM engine calls these during initialization and each frame.
 * Our versions avoid any framebuffer/display device access — all
 * rendering goes through DG_ScreenBuffer → DG_DrawFrame → Qt.
 * ============================================================ */

/**
 * Called once during engine startup.
 * Allocates the 8-bit indexed video buffer that DOOM renders to.
 */
void I_InitGraphics(void)
{
    fprintf(stderr, "[DOOM] I_InitGraphics: NERO Qt backend (no /dev/fb0)\n");
    // Allocate at DOOM's native render resolution (320x200), NOT at
    // DOOMGENERIC_RESX x DOOMGENERIC_RESY (640x400). The engine's V_Init
    // allocates screens[1..4] at 320x200, and wipe code calls I_ReadScreen
    // expecting a 320x200 buffer. Using 640x400 here would overflow those
    // destination buffers and crash during screen wipe transitions.
    I_VideoBuffer = (byte *)Z_Malloc(320 * 200, PU_STATIC, NULL);
    screens[0] = I_VideoBuffer;
    fprintf(stderr, "[DOOM] I_InitGraphics: allocated 320x200 buffer at %p\n",
            (void*)I_VideoBuffer);
}

void I_ShutdownGraphics(void)
{
    /* Z_Malloc'd memory is freed when the zone is destroyed */
}

void I_StartFrame(void)
{
    /* Nothing needed — Qt handles frame timing */
}

/**
 * Called each tick to process input events.
 * Reads keys from our queue via DG_GetKey and posts them
 * to the DOOM engine's event system via D_PostEvent.
 * Without this, keyboard/button input never reaches the game.
 */
void I_StartTic(void)
{
    int pressed;
    unsigned char doomKey;

    while (DG_GetKey(&pressed, &doomKey))
    {
        event_t event;
        event.type = pressed ? ev_keydown : ev_keyup;
        event.data1 = doomKey;
        event.data2 = -1;
        event.data3 = -1;
        D_PostEvent(&event);
    }
}

void I_UpdateNoBlit(void)
{
    /* Nothing needed */
}

/**
 * Called each frame after rendering is complete.
 * Converts the 8-bit indexed I_VideoBuffer to 32-bit XRGB in
 * DG_ScreenBuffer using the current palette, then calls DG_DrawFrame
 * which hands the frame to Qt.
 */
void I_FinishUpdate(void)
{
    if (!I_VideoBuffer || !DG_ScreenBuffer) return;

    // DOOM renders at 320x200 (SCREENWIDTH x SCREENHEIGHT) into I_VideoBuffer.
    // DG_ScreenBuffer is DOOMGENERIC_RESX x DOOMGENERIC_RESY (640x400).
    // Scale each pixel to a 2x2 block.
    for (int y = 0; y < 200; y++) {
        for (int x = 0; x < 320; x++) {
            uint32_t color = s_palette[I_VideoBuffer[y * 320 + x]];
            DG_ScreenBuffer[(y * 2) * DOOMGENERIC_RESX + (x * 2)]         = color;
            DG_ScreenBuffer[(y * 2) * DOOMGENERIC_RESX + (x * 2) + 1]     = color;
            DG_ScreenBuffer[(y * 2 + 1) * DOOMGENERIC_RESX + (x * 2)]     = color;
            DG_ScreenBuffer[(y * 2 + 1) * DOOMGENERIC_RESX + (x * 2) + 1] = color;
        }
    }
    DG_DrawFrame();
}

/**
 * Copy the current screen to a buffer (used for wipes/transitions).
 */
void I_ReadScreen(byte *scr)
{
    memcpy(scr, I_VideoBuffer, 320 * 200);
}

/**
 * Called when the engine changes the color palette.
 * Converts the 768-byte RGB palette (256 entries × 3 bytes) to
 * 32-bit XRGB values for fast lookup during I_FinishUpdate.
 */
void I_SetPalette(byte *palette)
{
    fprintf(stderr, "[DOOM] I_SetPalette called\n");
    for (int i = 0; i < 256; i++) {
        s_palette[i] = (0xFF << 24)
                      | (palette[i * 3 + 0] << 16)    /* R */
                      | (palette[i * 3 + 1] << 8)     /* G */
                      | (palette[i * 3 + 2]);          /* B */
    }
}

/* ============================================================
 * Additional i_video.c symbols required by the DOOM engine.
 * These are stubs — NERO doesn't need mouse, screensaver,
 * gamma, or disk icon support.
 * ============================================================ */

void I_BindVideoVariables(void) { /* nothing to bind */ }

void I_SetWindowTitle(const char *title)
{
    fprintf(stderr, "[DOOM] I_SetWindowTitle: %s\n", title);
}

void I_GraphicsCheckCommandLine(void) { /* no command line video args */ }

void I_SetGrabMouseCallback(void (*func)(boolean grab))
{
    (void)func; /* no mouse grabbing in NERO */
}

void I_EnableLoadingDisk(int xoffs, int yoffs)
{
    (void)xoffs;
    (void)yoffs;
}

void I_DisplayFPSDots(boolean dots_on)
{
    (void)dots_on;
}

boolean I_CheckIsScreensaver(void)
{
    return false;
}

int I_GetPaletteIndex(int r, int g, int b)
{
    /* Find closest palette entry — simple nearest match */
    int best = 0;
    int bestDist = 0x7FFFFFFF;
    for (int i = 0; i < 256; i++) {
        int pr = (s_palette[i] >> 16) & 0xFF;
        int pg = (s_palette[i] >> 8) & 0xFF;
        int pb = s_palette[i] & 0xFF;
        int dist = (r - pr) * (r - pr) + (g - pg) * (g - pg) + (b - pb) * (b - pb);
        if (dist < bestDist) {
            bestDist = dist;
            best = i;
        }
    }
    return best;
}

void I_BeginRead(void) { /* no loading disk icon */ }
void I_EndRead(void) { /* no loading disk icon */ }

} /* extern "C" */

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
 *
 * Thread lifecycle:
 *   - Created and started in DoomController::startGame()
 *   - The game loop runs in start() until m_running is set to false
 *   - DoomController::stopGame() sets m_running = false, causing the
 *     game loop to exit, which emits stopped() → triggers QThread::quit()
 *   - The QThread::finished signal triggers deleteLater() on this worker
 *   - DoomView.qml also calls stopGame() on destruction and when
 *     isFocused becomes false (navigating away from the DOOM page)
 *
 * This ensures the thread is always cleaned up when DOOM is not being
 * played — whether the user presses Esc, navigates away, or the view
 * is destroyed.
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

    // Enable autorun (tricks engine into always run)
    joybspeed = 29;

    m_running = true;
    emit started();

    qInfo() << "[DOOM] Engine running, entering game loop";

    // This loop blocks the worker thread until m_running is set to false
    // by stop(), which is called from DoomController::stopGame()
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
 *
 * Thread cleanup guarantees:
 *   1. stopGame() sets m_running=false → game loop exits → emits stopped()
 *   2. stopped() signal is connected to QThread::quit() → event loop ends
 *   3. QThread::finished is connected to worker->deleteLater() → worker freed
 *   4. stopGame() also calls m_gameThread->wait(3000) as a safety net
 *   5. If the thread still hasn't stopped after 3s, terminate() is called
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
    , m_lastButtonValue(-1)
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

    // Connect to model data changes for hardware button input.
    // We read the raw button value directly (not via the consume-on-read
    // getXButtonPressed() methods) because DOOM needs both press AND release
    // events, while the existing ButtonController pattern only fires on press.
    connect(m_model, &Model::onCurrentDataChange, this, &DoomController::onDataChanged);

    // Auto-release timer: the wheel hardware does NOT send button release
    // events to MQTT. Without this timer, keys would stay held in DOOM
    // forever after a single press. The timer sends keyup after 100ms,
    // giving DOOM a few frames to register the press as a tap.
    m_releaseTimer = new QTimer(this);
    m_releaseTimer->setSingleShot(true);
    m_releaseTimer->setInterval(100);
    connect(m_releaseTimer, &QTimer::timeout, this, &DoomController::autoRelease);
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

    // Reset button state on game start
    m_lastButtonValue = -1;

    m_gameThread = new QThread(this);
    m_worker = new DoomWorker(m_wadPath);
    m_worker->moveToThread(m_gameThread);

    // Wire up thread lifecycle:
    //   thread started → worker starts game loop
    //   worker stopped → thread quits its event loop
    //   thread finished → worker is deleted
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

    // Signal the game loop to exit
    m_worker->stop();

    if (m_gameThread) {
        // Ask the thread's event loop to quit
        m_gameThread->quit();

        // Wait up to 3 seconds for the game loop to finish
        m_gameThread->wait(3000);

        // If still running after 3s, force kill as a last resort
        if (m_gameThread->isRunning()) {
            qWarning() << "[DOOM] Force terminating game thread";
            m_gameThread->terminate();
            m_gameThread->wait(1000);
        }
    }

    m_running = false;
    m_lastButtonValue = -1;
    m_releaseTimer->stop();
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

/* ============================================================
 * Hardware button handling via MQTT
 *
 * The wheel hardware sends button presses but NOT releases.
 * The MQTT value stays at the pressed button ID forever.
 *
 * Approach:
 *   1. Detect value change → send DOOM keydown + start 100ms timer
 *   2. Timer fires → send DOOM keyup (auto-release)
 *   3. m_lastButtonValue stays at the button value to prevent
 *      phantom re-presses from the stale MQTT value
 *
 * To press the same button twice: press any other button first,
 * which changes the MQTT value, then press the original button.
 * This is a hardware limitation — without release events, we
 * cannot distinguish a stale value from a new press of the same button.
 *
 * MQTT values (physical button - 1):
 *   0=esc, 1=left, 2=mid-left, 3=up, 4=down, 5=enter, 6=right, 7=mid-right
 * ============================================================ */

void DoomController::onDataChanged()
{
    if (!m_running) return;

    std::optional<float> raw = m_model->getById(FORWARDBUTTON);
    if (!raw.has_value()) return;

    int currentValue = static_cast<int>(*raw);

    // No change from last processed value — nothing to do
    if (currentValue == m_lastButtonValue) return;

    // Escape button (0) — stop DOOM and go home immediately
    if (currentValue == BUTTON_VALUE_ESCAPE) {
        m_releaseTimer->stop();
        releaseCurrentButton();
        m_lastButtonValue = currentValue;
        stopGame();
        emit escapeRequested();
        return;
    }

    // Known button pressed → release old, press new
    if (isKnownDoomButton(currentValue)) {
        m_releaseTimer->stop();
        releaseCurrentButton();
        pressButton(currentValue);
        m_lastButtonValue = currentValue;

        // Tap buttons: auto-release after 100ms (single shot)
        // Toggle buttons: stay held until a different button is pressed
        if (isTapButton(currentValue)) {
            m_releaseTimer->start();
        }
    } else {
        // Non-button value (release event, if hardware sends one)
        m_releaseTimer->stop();
        releaseCurrentButton();
        m_lastButtonValue = currentValue;
    }
}

/**
 * Auto-release timer callback.
 * Sends keyup for the currently held button.
 * Does NOT reset m_lastButtonValue — the stale MQTT value must
 * not be re-detected as a new press.
 */
void DoomController::autoRelease()
{
    releaseCurrentButton();
    // m_lastButtonValue intentionally NOT reset.
    // It stays at the button value so the stale MQTT value
    // doesn't trigger phantom re-presses.
}

void DoomController::releaseCurrentButton()
{
    if (!isKnownDoomButton(m_lastButtonValue)) return;

    if (m_lastButtonValue == BUTTON_VALUE_ENTER) {
        sendKey(KEY_ENTER, false);
        sendKey(KEY_FIRE, false);
        sendKey(KEY_USE, false);
    } else {
        unsigned char key = mapButtonValueToDoomKey(m_lastButtonValue);
        if (key != 0) sendKey(key, false);
    }
}

void DoomController::pressButton(int value)
{
    if (value == BUTTON_VALUE_ENTER) {
        sendKey(KEY_ENTER, true);
        sendKey(KEY_FIRE, true);
        sendKey(KEY_USE, true);
    } else {
        unsigned char key = mapButtonValueToDoomKey(value);
        if (key != 0) sendKey(key, true);
    }
}

/**
 * Maps raw MQTT button values to DOOM key codes.
 * Values come from "Wheel/Buttons/button_id" topic.
 */
unsigned char DoomController::mapButtonValueToDoomKey(int value)
{
    switch (value) {
        case BUTTON_VALUE_LEFT:         return KEY_LEFTARROW;   // phys 2 — turn left
        case BUTTON_VALUE_MIDDLE_LEFT:  return KEY_LEFTARROW;   // phys 3 — also turn left
        case BUTTON_VALUE_UP:           return KEY_UPARROW;     // phys 4 — move forward
        case BUTTON_VALUE_DOWN:         return KEY_DOWNARROW;   // phys 5 — move backward
        case BUTTON_VALUE_ENTER:        return KEY_ENTER;       // phys 6 — handled specially
        case BUTTON_VALUE_RIGHT:        return KEY_RIGHTARROW;  // phys 7 — turn right
        case BUTTON_VALUE_MIDDLE_RIGHT: return KEY_RIGHTARROW;  // phys 8 — also turn right
        default: return 0;
    }
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
