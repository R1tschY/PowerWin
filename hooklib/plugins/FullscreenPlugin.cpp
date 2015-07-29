#include "FullscreenPlugin.h"

#include <functional>
#include <powrprof.h>
#include <string>
#include <type_traits>

#include "../windows/timeout.h"
#include "../windows/debug.h"
#include "../macros.h"

//
// Fullscreen Feature
//
// deactivate screensaver and standby-mode when a fullscreen window exists
//

FullscreenPlugin::FullscreenPlugin() :
  Plugin(L"fullscreen"),
  timeout_(std::bind(&FullscreenPlugin::onTimeout, this), -1)
{ }

void FullscreenPlugin::onActivate(const Options& options) {
  fullscreen_window_ = false;
  onTimeout();
  timeout_.setInterval(10);
}

void FullscreenPlugin::onDeactivate() {
  timeout_.setInterval(-1);
}

inline static
bool isScreensaverActive() {
  BOOL result;
  SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &result, 0);
  return result;
}

static
HWND getFullscreenWindow() {
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
      && app.left   <= desktop_rect.left
      && !isScreensaverActive())
  {
    return hwnd;
  } else {
    return nullptr;
  }
}

inline static
bool existsFullscreenWindow() {
  return getFullscreenWindow() != nullptr;
}

void FullscreenPlugin::onTimeout() {
  bool state = existsFullscreenWindow();

  // Wenn Fullscreenfenster vorhanden im Pollingmodus arbeiten (sollte zuverlässig auf allen Systemen funktionieren)
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
