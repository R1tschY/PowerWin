#include "ScrollPlugin.h"

#include <functional>

#include <app/macros.h>
#include <cpp-utils/algorithm/container.h>
#include <lightports/core.h>

ScrollPlugin::ScrollPlugin() :
  Plugin(L"scroll"),
  hook_(std::bind(&ScrollPlugin::handle, this, std::placeholders::_1, std::placeholders::_2))
{ }

void ScrollPlugin::onActivate(const Options& options) {
  inverse_ = getBooleanOption(L"inverse", false);

  hook_.activate();
}

void ScrollPlugin::onDeactivate() {
  hook_.deactivate();
}

__attribute__((hot))
bool ScrollPlugin::handle(POINT pt, int steps) {
  // get window under cursor
  HWND window = WindowFromPoint(pt);
  if (window == NULL) {
    OutputDebugString(L"ScrollPlugin: window == NULL\n");
    return false;
  }

  /*
    // get window class name
    wchar_t class_name[255];
    GetClassNameW(window, class_name, sizeof(class_name);
    
    // get window text
    wchar_t window_text[255];
    GetWindowTextW(window, window_text, sizeof(window_text));
    
    // get window class name
    wchar_t class_name_active[255];
    GetClassNameW(GetForegroundWindow(), class_name_active, sizeof(class_name_active));
    */

  if (inverse_) {
    steps = -steps;
  }
    
  // vkeys in WM_MOUSEWHEEL from no application used.
  int vkeys = 
      ((GetKeyState(VK_SHIFT) & SHIFTED) != 0) << 2
    | ((GetKeyState(VK_CONTROL) & SHIFTED) != 0) << 3;

  PostMessage(window, WM_MOUSEWHEEL, Windows::dword(steps, vkeys),
    Windows::dword(pt.y, pt.x));

  return true;
}