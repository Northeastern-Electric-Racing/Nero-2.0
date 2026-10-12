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
#include <QElapsedTimer>
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

// DOOM key per wheel button. Launch Control and Traction Control also toggle
// car settings on the VCU, so DOOM leaves them unmapped.
static unsigned char doomKeyFor(Buttons::Button button) {
  switch (button) {
  case Buttons::Left:
    return KEY_LEFTARROW;
  case Buttons::Right:
    return KEY_RIGHTARROW;
  case Buttons::UpRegen:
    return KEY_UPARROW;
  case Buttons::DownRegen:
    return KEY_DOWNARROW;
  default:
    return 0;
  }
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
 * DoomWorker
 * ============================================================ */

DoomWorker::DoomWorker(const QString &wadPath) : m_wadPath(wadPath) {}

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

  // The engine keeps argv for its lifetime, so it must outlive this call
  static QByteArray wadPathUtf8 = m_wadPath.toUtf8();
  static char *argv[] = {const_cast<char *>("nero_doom"),
                         const_cast<char *>("-iwad"), wadPathUtf8.data(),
                         nullptr};
  qInfo() << "[DOOM] Starting engine with WAD:" << m_wadPath;
  doomgeneric_Create(3, argv);

  // Enable autorun (tricks engine into always run)
  joybspeed = 29;

  // Each tick waits for DOOM's next frame itself, so tick as often as allowed
  m_tickTimer = new QTimer(this);
  connect(m_tickTimer, &QTimer::timeout, this, [] { doomgeneric_Tick(); });
  m_tickTimer->start(0);
}

void DoomWorker::setPaused(bool paused) {
  if (!m_tickTimer)
    return;
  if (paused)
    m_tickTimer->stop();
  else
    m_tickTimer->start(0);
}

/* ============================================================
 * DoomController
 * ============================================================ */

DoomController::DoomController(QObject *parent) : QObject(parent) {
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

  if (m_wadPath.isEmpty())
    qWarning() << "[DOOM] WAD file not found! Searched:" << wadSearchPaths;

  // The wheel sends no release, so Enter gets its key-up after 100 ms
  m_releaseTimer.setSingleShot(true);
  m_releaseTimer.setInterval(100);
  connect(&m_releaseTimer, &QTimer::timeout, this,
          &DoomController::releaseCurrentButton);
}

DoomController::~DoomController() {
  m_gameThread.quit();
  m_gameThread.wait();
}

DoomImageProvider *DoomController::createImageProvider() {
  m_imageProvider = new DoomImageProvider();
  return m_imageProvider;
}

void DoomController::resume() {
  if (m_wadPath.isEmpty())
    return;
  if (!m_worker) {
    m_worker = new DoomWorker(m_wadPath);
    m_worker->moveToThread(&m_gameThread);
    connect(m_worker, &DoomWorker::frameReady, this,
            &DoomController::onFrameReady);
    connect(&m_gameThread, &QThread::finished, m_worker, &QObject::deleteLater);
    m_gameThread.start();
    QMetaObject::invokeMethod(m_worker, &DoomWorker::start);
  } else {
    QMetaObject::invokeMethod(m_worker, &DoomWorker::setPaused, false);
  }
}

void DoomController::pause() {
  if (!m_worker)
    return;
  m_releaseTimer.stop();
  releaseCurrentButton();
  QMetaObject::invokeMethod(m_worker, &DoomWorker::setPaused, true);
}

void DoomController::press(Buttons::Button button) {
  if (!m_worker)
    return;

  m_releaseTimer.stop();
  releaseCurrentButton();

  // Esc is the VCU's: it goes home, which pauses DOOM
  if (button != Buttons::Enter && doomKeyFor(button) == 0)
    return;

  sendButton(button, true);
  m_heldButton = button;
  if (button == Buttons::Enter)
    m_releaseTimer.start();
}

void DoomController::releaseCurrentButton() {
  if (!m_heldButton)
    return;
  sendButton(*m_heldButton, false);
  m_heldButton.reset();
}

void DoomController::sendButton(Buttons::Button button, bool pressed) {
  if (button == Buttons::Enter) {
    m_worker->enqueueKey({pressed, KEY_ENTER});
    m_worker->enqueueKey({pressed, KEY_FIRE});
    m_worker->enqueueKey({pressed, KEY_USE});
  } else {
    m_worker->enqueueKey({pressed, doomKeyFor(button)});
  }
}

void DoomController::onFrameReady(const QImage &frame) {
  if (m_imageProvider) {
    m_imageProvider->updateFrame(frame);
  }
  m_frameCounter++;
  emit frameCounterChanged();
}
