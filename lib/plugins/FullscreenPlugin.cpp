#include "FullscreenPlugin.h"

#include <boost/bind.hpp>
#include <powrprof.h>
#include <string>
#include <type_traits>
#include <boost/lexical_cast.hpp>

#include "../windows/timeout.h"
#include "../Utils.h"
#include "../macros.h"

static LRESULT CALLBACK fullscreen_proc(int code, WPARAM wparam, LPARAM lparam);
static DLL_SHARED Hook hook(WH_CBT, Hook::THREAD_ID_ALL_THREADS, fullscreen_proc);

FullscreenPlugin::FullscreenPlugin() :
  Plugin(L"fullscreen"),
  timeout_(boost::bind(&FullscreenPlugin::onTimeout, this), -1)
{ }

void FullscreenPlugin::onActivate(const Options& options) {
  fullscreen_window_ = false;
  onTimeout();
  timeout_.setInterval(1);

//  hook.activate();
}

void FullscreenPlugin::onDeactivate() {
//  hook.deactivate();
  timeout_.setInterval(-1);
}

///////////////////////////////////////////////////////////////////////////////
// Fullscreen (Deaktiviere Screensaver bei aktivien Vollbildanwendungen)

inline
bool is_screensaver_active() {
  BOOL result;
  SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &result, 0);
  return result;
}

static
HWND get_fullscreen_window() {
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
      && !is_screensaver_active())
  {
    return hwnd;
  } else {
    return nullptr;
  }
}

inline
bool exist_fullscreen_window() {
  return get_fullscreen_window() != nullptr;
}

void FullscreenPlugin::onTimeout() {
  bool new_state = exist_fullscreen_window();

  // Wenn Fullscreenfenster vorhanden im Pollingmodus arbeiten (sollte zuverlässig auf allen Systemen funktionieren)
  if (new_state) {
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
  }
  
  if (new_state != fullscreen_window_) {
    fullscreen_window_ = new_state;
    if (!new_state) {
      // Standby wieder zulassen
      SetThreadExecutionState(ES_CONTINUOUS);
    }
  }
//  if (isActive()) {
//    OutputDebugString(L"Fullscreen Window: yes\n");
//  } else {
//    OutputDebugString(L"Fullscreen Window: no\n");
//  }
}

static __attribute__((hot))
LRESULT CALLBACK fullscreen_proc(int code, WPARAM wparam, LPARAM lparam) {
  if (code == HCBT_SYSCOMMAND
    && (wparam == SC_SCREENSAVE || (wparam == SC_MONITORPOWER && lparam != -1))
    && exist_fullscreen_window())
  {
    return 1;
  }

  return CallNextHookEx(hook.getHandle(), code, wparam, lparam);
}
