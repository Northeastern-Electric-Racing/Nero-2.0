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
#include <QDebug>
#include <QDir>
#include <QFile>

#include <cstdio>
#include <cstring>

/* ============================================================
 * doomgeneric C headers
 * Source: https://github.com/ozkl/doomgeneric/tree/master/doomgeneric
 * These headers are from the upstream repo and should not be modified.
 * ============================================================ */
extern "C" {
#include "d_event.h"
#include "doomgeneric.h"
#include "doomkeys.h"
#include "doomtype.h"
#include "z_zone.h"

extern int joybspeed;
extern void D_PostEvent(event_t *ev);
}

/* ============================================================
 * Wheel buttons are Buttons::Button values (buttons.h has the layout).
 * DOOM uses the eight face buttons, Esc through TractionControl.
 * ============================================================ */

/**
 * Returns true if the value corresponds to a known DOOM-mapped button.
 * Any value NOT in this set (e.g. -1, 8, 9) is treated as a release.
 */
static bool isKnownDoomButton(int value) {
  return value >= Buttons::Esc && value <= Buttons::TractionControl;
}

/**
 * Returns true if the button should be a brief tap (auto-release after timer).
 * These buttons send keydown then keyup after 100ms:
 *   - LaunchControl (middle left): small left nudge
 *   - TractionControl (middle right): small right nudge
 *   - Enter: shoot once / use once / menu select
 *
 * All other buttons are TOGGLE — they stay held until a different
 * button is pressed:
 *   - Left: continuous turn left
 *   - Right: continuous turn right
 *   - UpRegen: continuous move forward
 *   - DownRegen: continuous move backward
 */
static bool isTapButton(int value) {
  return value == Buttons::LaunchControl || value == Buttons::TractionControl ||
         value == Buttons::Enter;
}

/* ============================================================
 * Platform bridge — static state for C callbacks
 * ============================================================ */
static DoomWorker *g_doomWorker = nullptr;

static void platform_frame_callback(uint32_t *framebuffer, int width,
                                    int height);
static int platform_getkey_callback(unsigned char *pressed,
                                    unsigned char *doomKey);

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
byte *screens[5] = {NULL, NULL, NULL, NULL, NULL};

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

void DG_Init(void) {
  fprintf(stderr, "[DOOM] Platform initialized (NERO Qt backend)\n");
}

void DG_DrawFrame(void) {
  platform_frame_callback(DG_ScreenBuffer, DOOMGENERIC_RESX, DOOMGENERIC_RESY);
}

void DG_SleepMs(uint32_t ms) { QThread::msleep(ms); }

uint32_t DG_GetTicksMs(void) {
  static QElapsedTimer timer;
  static bool started = false;
  if (!started) {
    timer.start();
    started = true;
  }
  return static_cast<uint32_t>(timer.elapsed());
}

int DG_GetKey(int *pressed, unsigned char *doom_key) {
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

void DG_SetWindowTitle(const char *title) {
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
void I_InitGraphics(void) {
  fprintf(stderr, "[DOOM] I_InitGraphics: NERO Qt backend (no /dev/fb0)\n");
  // Allocate at DOOM's native render resolution (320x200), NOT at
  // DOOMGENERIC_RESX x DOOMGENERIC_RESY (640x400). The engine's V_Init
  // allocates screens[1..4] at 320x200, and wipe code calls I_ReadScreen
  // expecting a 320x200 buffer. Using 640x400 here would overflow those
  // destination buffers and crash during screen wipe transitions.
  I_VideoBuffer = (byte *)Z_Malloc(320 * 200, PU_STATIC, NULL);
  screens[0] = I_VideoBuffer;
  fprintf(stderr, "[DOOM] I_InitGraphics: allocated 320x200 buffer at %p\n",
          (void *)I_VideoBuffer);
}

void I_ShutdownGraphics(void) {
  /* Z_Malloc'd memory is freed when the zone is destroyed */
}

void I_StartFrame(void) { /* Nothing needed — Qt handles frame timing */ }

/**
 * Called each tick to process input events.
 * Reads keys from our queue via DG_GetKey and posts them
 * to the DOOM engine's event system via D_PostEvent.
 * Without this, keyboard/button input never reaches the game.
 */
void I_StartTic(void) {
  int pressed;
  unsigned char doomKey;

  while (DG_GetKey(&pressed, &doomKey)) {
    event_t event;
    event.type = pressed ? ev_keydown : ev_keyup;
    event.data1 = doomKey;
    event.data2 = -1;
    event.data3 = -1;
    D_PostEvent(&event);
  }
}

void I_UpdateNoBlit(void) { /* Nothing needed */ }

/**
 * Called each frame after rendering is complete.
 * Converts the 8-bit indexed I_VideoBuffer to 32-bit XRGB in
 * DG_ScreenBuffer using the current palette, then calls DG_DrawFrame
 * which hands the frame to Qt.
 */
void I_FinishUpdate(void) {
  if (!I_VideoBuffer || !DG_ScreenBuffer)
    return;

  // DOOM renders at 320x200 (SCREENWIDTH x SCREENHEIGHT) into I_VideoBuffer.
  // DG_ScreenBuffer is DOOMGENERIC_RESX x DOOMGENERIC_RESY (640x400).
  // Scale each pixel to a 2x2 block.
  for (int y = 0; y < 200; y++) {
    for (int x = 0; x < 320; x++) {
      uint32_t color = s_palette[I_VideoBuffer[y * 320 + x]];
      DG_ScreenBuffer[(y * 2) * DOOMGENERIC_RESX + (x * 2)] = color;
      DG_ScreenBuffer[(y * 2) * DOOMGENERIC_RESX + (x * 2) + 1] = color;
      DG_ScreenBuffer[(y * 2 + 1) * DOOMGENERIC_RESX + (x * 2)] = color;
      DG_ScreenBuffer[(y * 2 + 1) * DOOMGENERIC_RESX + (x * 2) + 1] = color;
    }
  }
  DG_DrawFrame();
}

/**
 * Copy the current screen to a buffer (used for wipes/transitions).
 */
void I_ReadScreen(byte *scr) { memcpy(scr, I_VideoBuffer, 320 * 200); }

/**
 * Called when the engine changes the color palette.
 * Converts the 768-byte RGB palette (256 entries × 3 bytes) to
 * 32-bit XRGB values for fast lookup during I_FinishUpdate.
 */
void I_SetPalette(byte *palette) {
  fprintf(stderr, "[DOOM] I_SetPalette called\n");
  for (int i = 0; i < 256; i++) {
    s_palette[i] = (0xFF << 24) | (palette[i * 3 + 0] << 16) /* R */
                   | (palette[i * 3 + 1] << 8)               /* G */
                   | (palette[i * 3 + 2]);                   /* B */
  }
}

/* ============================================================
 * Additional i_video.c symbols required by the DOOM engine.
 * These are stubs — NERO doesn't need mouse, screensaver,
 * gamma, or disk icon support.
 * ============================================================ */

void I_BindVideoVariables(void) { /* nothing to bind */ }

void I_SetWindowTitle(const char *title) {
  fprintf(stderr, "[DOOM] I_SetWindowTitle: %s\n", title);
}

void I_GraphicsCheckCommandLine(void) { /* no command line video args */ }

void I_SetGrabMouseCallback(void (*func)(boolean grab)) {
  (void)func; /* no mouse grabbing in NERO */
}

void I_EnableLoadingDisk(int xoffs, int yoffs) {
  (void)xoffs;
  (void)yoffs;
}

void I_DisplayFPSDots(boolean dots_on) { (void)dots_on; }

boolean I_CheckIsScreensaver(void) { return false; }

int I_GetPaletteIndex(int r, int g, int b) {
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

static void platform_frame_callback(uint32_t *framebuffer, int width,
                                    int height) {
  if (!g_doomWorker)
    return;

  QImage frame(reinterpret_cast<const uchar *>(framebuffer), width, height,
               width * static_cast<int>(sizeof(uint32_t)),
               QImage::Format_RGB32);

  emit g_doomWorker->frameReady(frame.copy());
}

static int platform_getkey_callback(unsigned char *pressed,
                                    unsigned char *doomKey) {
  if (!g_doomWorker)
    return 0;
  return g_doomWorker->dequeueKey(pressed, doomKey);
}

/* ============================================================
 * DoomImageProvider
 * ============================================================ */

DoomImageProvider::DoomImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image),
      m_currentFrame(320, 200, QImage::Format_RGB32) {
  m_currentFrame.fill(Qt::black);
}

QImage DoomImageProvider::requestImage(const QString &id, QSize *size,
                                       const QSize &requestedSize) {
  Q_UNUSED(id)
  QMutexLocker lock(&m_mutex);

  if (size)
    *size = m_currentFrame.size();

  if (requestedSize.isValid() && requestedSize != m_currentFrame.size())
    return m_currentFrame.scaled(requestedSize, Qt::KeepAspectRatio,
                                 Qt::SmoothTransformation);

  return m_currentFrame;
}

void DoomImageProvider::updateFrame(const QImage &frame) {
  QMutexLocker lock(&m_mutex);
  m_currentFrame = frame;
}

/* ============================================================
 * DoomWorker — Runs the game loop on a dedicated thread
 *
 * Thread lifecycle:
 *   - Created and started in DoomController::startGame()
 *   - The game loop runs in start() until m_stopRequested is set
 *   - DoomController::stopGame() sets m_stopRequested, even while the
 *     engine is still loading, so the game loop exits (or never starts),
 *     which emits stopped() → triggers QThread::quit()
 *   - The QThread::finished signal triggers deleteLater() on this worker
 *   - DoomView.qml also calls stopGame() on destruction (e.g. Esc takes
 *     the VCU home)
 *
 * This ensures the thread is always cleaned up when DOOM is not being
 * played.
 * ============================================================ */

DoomWorker::DoomWorker(const QString &wadPath, QObject *parent)
    : QObject(parent), m_wadPath(wadPath) {}

DoomWorker::~DoomWorker() { stop(); }

void DoomWorker::enqueueKey(const DoomKeyEvent &event) {
  QMutexLocker lock(&m_keyMutex);
  if (m_keyQueue.size() < 64) {
    m_keyQueue.enqueue(event);
  }
}

int DoomWorker::dequeueKey(unsigned char *pressed, unsigned char *doomKey) {
  QMutexLocker lock(&m_keyMutex);
  if (m_keyQueue.isEmpty())
    return 0;

  DoomKeyEvent event = m_keyQueue.dequeue();
  *pressed = event.pressed;
  *doomKey = event.doomKey;
  return 1;
}

void DoomWorker::start() {
  g_doomWorker = this;

  QByteArray wadPathUtf8 = m_wadPath.toUtf8();

  char *argv[] = {const_cast<char *>("nero_doom"), const_cast<char *>("-iwad"),
                  wadPathUtf8.data(), nullptr};
  int argc = 3;

  qInfo() << "[DOOM] Starting engine with WAD:" << m_wadPath;

  doomgeneric_Create(argc, argv);

  // Enable autorun (tricks engine into always run)
  joybspeed = 29;

  // stop() may already have been called while the engine loaded
  if (!m_stopRequested) {
    emit started();
    qInfo() << "[DOOM] Engine running, entering game loop";
  }

  // This loop blocks the worker thread until stop(), which is called from
  // DoomController::stopGame()
  while (!m_stopRequested) {
    doomgeneric_Tick();
  }

  qInfo() << "[DOOM] Game loop exited";
  g_doomWorker = nullptr;
  emit stopped();
}

void DoomWorker::stop() { m_stopRequested = true; }

/* ============================================================
 * DoomController — Main QML-facing controller
 *
 * Thread cleanup guarantees:
 *   1. stopGame() acts whenever a thread exists, loading or running, and
 *      asks the worker to stop → game loop exits → emits stopped()
 *   2. stopped() signal is connected to QThread::quit() → event loop ends
 *   3. QThread::finished is connected to deleteLater() on the worker and the
 *      thread → both freed
 *   4. stopGame() also calls m_gameThread->wait(3000) as a safety net
 *   5. If the thread still hasn't stopped after 3s, terminate() is called
 * ============================================================ */

DoomController::DoomController(QObject *parent)
    : QObject(parent), m_worker(nullptr), m_gameThread(nullptr),
      m_imageProvider(nullptr), m_running(false), m_frameCounter(0),
      m_statusText("Press ENTER to start DOOM"), m_lastButtonValue(-1) {
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

  // Auto-release timer: the wheel hardware does NOT send button release
  // events to MQTT. Without this timer, keys would stay held in DOOM
  // forever after a single press. The timer sends keyup after 100ms,
  // giving DOOM a few frames to register the press as a tap.
  m_releaseTimer = new QTimer(this);
  m_releaseTimer->setSingleShot(true);
  m_releaseTimer->setInterval(100);
  connect(m_releaseTimer, &QTimer::timeout, this, &DoomController::autoRelease);
}

DoomController::~DoomController() { stopGame(); }

DoomImageProvider *DoomController::createImageProvider() {
  m_imageProvider = new DoomImageProvider();
  return m_imageProvider;
}

void DoomController::startGame() {
  // One game thread at a time, including while it loads
  if (m_gameThread)
    return;

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
  //   thread finished → worker and thread are deleted
  connect(m_gameThread, &QThread::started, m_worker, &DoomWorker::start);
  connect(m_worker, &DoomWorker::frameReady, this,
          &DoomController::onFrameReady, Qt::QueuedConnection);
  connect(m_worker, &DoomWorker::started, this,
          &DoomController::onWorkerStarted, Qt::QueuedConnection);
  connect(m_worker, &DoomWorker::stopped, this,
          &DoomController::onWorkerStopped, Qt::QueuedConnection);

  connect(m_worker, &DoomWorker::stopped, m_gameThread, &QThread::quit);
  connect(m_gameThread, &QThread::finished, m_worker, &QObject::deleteLater);
  connect(m_gameThread, &QThread::finished, m_gameThread,
          &QObject::deleteLater);

  m_gameThread->start();
}

void DoomController::stopGame() {
  // Not `running`: that only turns true once the engine has loaded, and a
  // thread still loading must be stopped too
  if (!m_gameThread)
    return;

  qInfo() << "[DOOM] Stopping game...";

  // Signal the game loop to exit
  m_worker->stop();

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

  // Both delete themselves once the thread finishes; deleting a thread that
  // is somehow still running would abort NERO
  m_gameThread = nullptr;
  m_worker = nullptr;

  m_running = false;
  m_lastButtonValue = -1;
  m_releaseTimer->stop();
  m_statusText = "Press ENTER to start DOOM";
  emit runningChanged();
  emit statusTextChanged();
}

void DoomController::sendKey(int doomKeyCode, bool pressed) {
  if (!m_worker || !m_running)
    return;

  DoomKeyEvent event;
  event.pressed = pressed ? 1 : 0;
  event.doomKey = static_cast<unsigned char>(doomKeyCode);
  m_worker->enqueueKey(event);
}

/* ============================================================
 * Hardware button handling
 *
 * The wheel sends presses but NOT releases, so each press releases the
 * previous button first. Tap buttons auto-release after 100ms; toggle
 * buttons stay held until the next press.
 * ============================================================ */

void DoomController::press(Buttons::Button button) {
  if (!m_running)
    return;

  m_releaseTimer->stop();
  releaseCurrentButton();
  m_lastButtonValue = button;

  // Esc belongs to the VCU: it goes home, which closes the page and stops DOOM
  if (!isKnownDoomButton(button) || button == Buttons::Esc)
    return;

  pressButton(button);
  if (isTapButton(button))
    m_releaseTimer->start();
}

/** Auto-release timer callback: sends keyup for the held button. */
void DoomController::autoRelease() { releaseCurrentButton(); }

void DoomController::releaseCurrentButton() {
  if (!isKnownDoomButton(m_lastButtonValue))
    return;

  if (m_lastButtonValue == Buttons::Enter) {
    sendKey(KEY_ENTER, false);
    sendKey(KEY_FIRE, false);
    sendKey(KEY_USE, false);
  } else {
    unsigned char key = mapButtonValueToDoomKey(m_lastButtonValue);
    if (key != 0)
      sendKey(key, false);
  }
}

void DoomController::pressButton(int value) {
  if (value == Buttons::Enter) {
    sendKey(KEY_ENTER, true);
    sendKey(KEY_FIRE, true);
    sendKey(KEY_USE, true);
  } else {
    unsigned char key = mapButtonValueToDoomKey(value);
    if (key != 0)
      sendKey(key, true);
  }
}

/**
 * Maps wheel buttons to DOOM key codes.
 */
unsigned char DoomController::mapButtonValueToDoomKey(int value) {
  switch (value) {
  case Buttons::Left:
    return KEY_LEFTARROW; // turn left
  case Buttons::LaunchControl:
    return KEY_LEFTARROW; // middle left — also turn left
  case Buttons::UpRegen:
    return KEY_UPARROW; // move forward
  case Buttons::DownRegen:
    return KEY_DOWNARROW; // move backward
  case Buttons::Enter:
    return KEY_ENTER; // handled specially
  case Buttons::Right:
    return KEY_RIGHTARROW; // turn right
  case Buttons::TractionControl:
    return KEY_RIGHTARROW; // middle right — also turn right
  default:
    return 0;
  }
}

void DoomController::onFrameReady(const QImage &frame) {
  if (m_imageProvider) {
    m_imageProvider->updateFrame(frame);
  }
  m_frameCounter++;
  emit frameCounterChanged();
}

void DoomController::onWorkerStarted() {
  m_running = true;
  m_statusText = "DOOM is running";
  emit runningChanged();
  emit statusTextChanged();
}

void DoomController::onWorkerStopped() {
  m_running = false;
  m_statusText = "Press ENTER to start DOOM";
  emit runningChanged();
  emit statusTextChanged();
}
