
#include "StdAfx.h"
#include "Hotkey.h"
#include "DesktopHooks.h"

static ATOM registerUtilWindow() {
  WNDCLASSW wc;

  wc.style         = 0;
  wc.lpfnWndProc   = DefWindowProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = WinExtra::getDllInstance();
  wc.hIcon         = 0;
  wc.hCursor       = 0;
  wc.hbrBackground = 0;
  wc.lpszMenuName  = 0;
  wc.lpszClassName = L"UtilWindow";

  return RegisterClassW(&wc);
}

HWND createUtilWindow(WNDPROC wndproc) {
  static ATOM wndclass = registerUtilWindow();
  HWND result;

  UNREFERENCED_PARAMETER(wndclass);

  result = CreateWindowExW(WS_EX_TOOLWINDOW, L"UtilWindow",
    L"", WS_POPUP, 0, 0, 0, 0, 0, 0, WinExtra::getDllInstance(), nullptr);
  if (wndproc)
    SetWindowLongPtr(result, GWLP_WNDPROC, (LONG_PTR)wndproc);

  return result;
}

LRESULT CALLBACK HotkeyWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
  case WM_HOTKEY:
    if (wparam > 0) {
      if (Hotkey::handlers[wparam](LOWORD(lparam), HIWORD(lparam))) {
        return 0;
      }
    }
    break;

  case WM_CREATE:
    // Initialize the window.
  case WM_DESTROY:
    // Clean up window-specific data objects.

  default:
    break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}
static const HWND hotkeywindow = createUtilWindow(HotkeyWndProc);

///////////////////////////////////////////////////////////////////////////////
// Hotkey

unsigned Hotkey::counter = 0;
std::vector<Hotkey::Handler> Hotkey::handlers;

void Hotkey::setHandler(unsigned id, Handler handler) {
  if (id >= handlers.size()) {
    handlers.resize(id+1, NULL);
  }
  handlers[id] = handler;
}

Hotkey::Hotkey(UINT modifiers, UINT vk, Handler handler) :
  modifiers_(modifiers), vk_(vk), handler_(handler), active_(false), id_(++counter)
{ }

Hotkey::Hotkey(Hotkey&& other) noexcept :
  modifiers_(other.modifiers_), vk_(other.vk_), handler_(other.handler_),
  active_(other.active_), id_(other.id_)
{
  other.active_ = false;
}

 Hotkey::~Hotkey() {
   deactivate();
 }

bool Hotkey::activate() {
  if (!active_) {
    setHandler(id_, handler_);
    BOOL success = RegisterHotKey(hotkeywindow, id_, modifiers_, vk_);
    if (!success) {
      //CRITICAL(L"RegisterHotKey failed: %s", GetLastWindowsError().c_str());
      return false;
    } else {
      return active_ = true;
    }
  } else {
    return true;
  }
}

bool Hotkey::deactivate() {
  if (active_ && UnregisterHotKey(hotkeywindow, id_)) {
    active_ = false;
    return true;
  } else {
    return true;
  }
}
