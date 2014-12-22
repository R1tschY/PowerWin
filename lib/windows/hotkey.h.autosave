#pragma once

#include <windows.h>
#include <functional>
#include <map>

#include "macros.h"
#include "controls/messagesink.h"

namespace Windows {

struct ShortCut {
  UINT modifiers;
  UINT key;
};

typedef std::function<void()> HotkeyHandler;

class Hotkey {
  DISALLOW_COPY_AND_ASSIGN(Hotkey);

public:
  Hotkey(unsigned fsModifiers, unsigned vk, HotkeyHandler handler);
  Hotkey(Hotkey&& other) noexcept;
  ~Hotkey();

  bool activate();
  bool deactivate();

  bool isActive() const { return active_; }

private:
  const unsigned modifiers_;
  const unsigned vk_;
  const HotkeyHandler handler_;
  const unsigned id_;

  bool active_;

  friend class HotkeyManager;
};

} // namespace Windows

