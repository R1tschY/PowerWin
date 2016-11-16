#include <functional>
#include <cpp-utils/algorithm/container.h>
#include <lightports/core.h>
#include <modules/scroll/scrollplugin.h>

static ScrollPlugin* instance = nullptr;

ScrollPlugin::ScrollPlugin(PowerWin::ModuleContext& context)
{
  cpp_assert(instance == nullptr);

  auto& config = context.getConfiguration();
  inverse_ = config.readBoolean(L"scroll", L"inverse", false);

  instance = this;
  hook_.create(WH_MOUSE_LL, Windows::Hook::AllThreads, &ScrollPlugin::hookProc);
}

ScrollPlugin::~ScrollPlugin()
{
  instance = nullptr;
}

__attribute__((hot))
LRESULT CALLBACK ScrollPlugin::hookProc(int code, WPARAM wparam, LPARAM lparam)
{
  if (code != HC_ACTION) {
    return instance->hook_.callNext(code, wparam, lparam);
  }

  MSLLHOOKSTRUCT* data = reinterpret_cast<MSLLHOOKSTRUCT*>(lparam);
  bool processed = instance->handle(data->pt, HIWORD(data->mouseData));
  if (!processed) {
    return instance->hook_.callNext(code, wparam, lparam);
  } else {
    return 1;
  }
}

__attribute__((hot))
bool ScrollPlugin::handle(POINT pt, int steps)
{
  // get window under cursor
  HWND window = ::WindowFromPoint(pt);
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

  ::PostMessageW(window, WM_MOUSEWHEEL, MAKELONG(vkeys, steps),
      MAKELONG(pt.x, pt.y));

  return true;
}

PowerWin::ModuleRegistry::element<ScrollPlugin> ScrollModule(
  L"scroll", L"scroll in inactive windows"
);
