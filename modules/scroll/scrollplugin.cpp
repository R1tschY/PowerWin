#include <functional>
#include <cpp-utils/algorithm/container.h>
#include <lightports/core.h>
#include <modules/scroll/scrollplugin.h>

ScrollPlugin::ScrollPlugin() :
  hook_([=](POINT pt, int steps){ return handle(pt, steps); })
{ }

void ScrollPlugin::activate(PowerWin::ModuleContext& context)
{
  auto& config = context.getConfiguration();

  inverse_ = config.readBoolean(L"scroll", L"inverse", false);

  hook_.activate();
}

void ScrollPlugin::deactivate() {
  hook_.deactivate();
}

__attribute__((hot))
bool ScrollPlugin::handle(POINT pt, int steps) {
  // get window under cursor
  HWND window = WindowFromPoint(pt);
  if (window == NULL) {
    OutputDebugStringW(L"ScrollPlugin: window == NULL\n");
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

PowerWin::ModuleRegistry::element<ScrollPlugin> ScrollModule(
  L"scroll", L"scroll in inactive windows"
);
