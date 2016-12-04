#include <functional>
#include <cpp-utils/algorithm/container.h>
#include <lightports/core.h>
#include <lightports/user/application.h>
#include <lightports/controls/control.h>
#include <modules/scroll/scrollplugin.h>
#include <app/mousehook.h>

using namespace Windows;

ScrollPlugin::ScrollPlugin(PowerWin::ModuleContext& context)
{
  auto& config = context.getConfiguration();
  inverse_ = config.readBoolean(L"scroll", L"inverse", false);

  connect(context.getMouseHook().mouseWheel(),
    [=](POINT pt, int steps, DWORD time){ return handle(pt, steps, time); });
}

__attribute__((hot))
bool ScrollPlugin::handle(POINT pt, int steps, DWORD time)
{
  // get window under cursor
  Window window = Window::at(pt);
  if (window == NULL)
  {
    ::OutputDebugStringW(L"ScrollPlugin: window == NULL\n");
    return false;
  }

  if (inverse_)
  {
    steps = -steps;
  }
    
  int vkeys = 
      ((::GetKeyState(VK_SHIFT) & SHIFTED) != 0) << 2
    | ((::GetKeyState(VK_CONTROL) & SHIFTED) != 0) << 3;

  ::PostMessageW(window.getHWND(), WM_MOUSEWHEEL, MAKELONG(vkeys, steps),
      MAKELONG(pt.x, pt.y));

  return true;
}

PowerWin::ModuleRegistry::element<ScrollPlugin> ScrollModule(
  L"scroll", L"scroll in inactive windows"
);
