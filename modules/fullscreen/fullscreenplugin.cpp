#include <functional>
#include <windows.h>
#include <powrprof.h>
#include <lightports/base/timeout.h>
#include <lightports/core.h>
#include <modules/fullscreen/fullscreenplugin.h>

//
// Fullscreen Feature
//
// deactivate screensaver and standby-mode when a fullscreen window exists
//

FullscreenPlugin::FullscreenPlugin() :
  timeout_(std::bind(&FullscreenPlugin::update, this), 59000),
  fullscreen_window_(false)
{ }

void FullscreenPlugin::activate(PowerWin::ModuleContext& context)
{
  fullscreen_window_ = false;
  update();
  timeout_.start();
}

void FullscreenPlugin::deactivate()
{
  timeout_.stop();
}

inline static
bool isScreensaverActive()
{
  BOOL result;
  SystemParametersInfoW(SPI_GETSCREENSAVERRUNNING, 0, &result, 0);
  return result;
}

static
HWND getFullscreenWindow()
{
  RECT desktop_rect;
  HWND desktop_window = GetDesktopWindow();
  RECT app;
  HWND hwnd = GetForegroundWindow();

  if (   desktop_window != nullptr
      && hwnd != nullptr
      && hwnd != desktop_window
      && hwnd != GetShellWindow()
      && GetWindowRect(desktop_window, &desktop_rect)
      && GetWindowRect(hwnd, &app)
      && app.top    <= desktop_rect.top
      && app.bottom >= desktop_rect.bottom
      && app.right  >= desktop_rect.right
      && app.left   <= desktop_rect.left // TODO: Rect.isWarping()
      && !isScreensaverActive())
  {
    return hwnd;
  } else {
    return nullptr;
  }
}

inline static
bool existsFullscreenWindow()
{
  return getFullscreenWindow() != nullptr;
}

void FullscreenPlugin::update()
{
  bool state = existsFullscreenWindow();

  // if fullscreen window exists, work in polling mode.
  if (state) {
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
  }
  
  if (state != fullscreen_window_) {
    fullscreen_window_ = state;

    if (!state) {
      // if no fullscreen window exists anymore, then activate normal mode.
      SetThreadExecutionState(ES_CONTINUOUS);
    }
  }
}

PowerWin::ModuleRegistry::element<FullscreenPlugin> FullscreenModule(
  L"fullscreen", L"inactivate stand-by and screensaver if fullscreen window exists"
);
