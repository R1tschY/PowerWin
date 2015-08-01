#pragma once

#include <windows.h>

#include "../core/macros.h"
#include "../extra/shortcut.h"

namespace Windows {

class Hotkey
{
  DISALLOW_COPY_AND_ASSIGN(Hotkey);

public:
  Hotkey(unsigned fsModifiers, unsigned vk, HWND handler, int id);
  Hotkey(Hotkey&& other) noexcept;
  ~Hotkey();

  bool activate();
  bool deactivate();

  bool isActive() const { return active_; }

  unsigned getModifiers() const { return modifiers_; }
  unsigned getVirtualKey() const { return vk_; }

private:
  // TODO: use Shortcut
  const unsigned modifiers_;
  const unsigned vk_;
  const HWND handler_;
  const int id_;

  bool active_;
};

} // namespace Windows

