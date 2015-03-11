#include "hotkey.h"
#include "../windows/utilwindow.h"
#include "../windows/debug.h"

namespace Windows {

Hotkey::Hotkey(unsigned modifiers, unsigned vk, HWND handler, int id) :
  modifiers_(modifiers), vk_(vk), handler_(handler), id_(id), active_(false)
{ }

Hotkey::Hotkey(Hotkey&& other) noexcept :
  modifiers_(other.modifiers_), vk_(other.vk_), handler_(other.handler_), id_(other.id_),
  active_(other.active_)
{
  other.active_ = false;
}

Hotkey::~Hotkey() {
  deactivate();
}

bool Hotkey::activate() {
  if (!active_) {
    bool success = win_print_on_fail(RegisterHotKey(handler_, id_, modifiers_, vk_));
    if (!success) {
      return false;
    }
    active_ = true;
  }
  return true;
}

bool Hotkey::deactivate() {
  if (active_ && win_print_on_fail(UnregisterHotKey(handler_, id_))) {
    active_ = false;
  }
  return true;
}

} // namespace Windows


