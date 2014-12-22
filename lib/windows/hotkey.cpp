#include "Hotkey.h"
#include "../windows/utilwindow.h"
#include "../windows/debug.h"

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
static HWND hotkeywindow = nullptr;

///////////////////////////////////////////////////////////////////////////////
// Hotkey

unsigned Hotkey::counter = 0;
std::vector<Hotkey::Handler> Hotkey::handlers; // FIXME

void Hotkey::setHandler(unsigned id, Handler handler) {
  if (id >= handlers.size()) {
    handlers.resize(id+1, nullptr);
  }
  handlers[id] = handler;
}

Hotkey::Hotkey(unsigned modifiers, unsigned vk, Handler handler) :
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

    if (hotkeywindow == nullptr) {
      hotkeywindow = Windows::createUtilWindow(HotkeyWndProc);
    }

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
