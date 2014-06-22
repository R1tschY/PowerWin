#pragma once

#include <windows.h>
#include <functional>
#include <vector>

#include "macros.h"


class Hotkey {
  DISALLOW_COPY_AND_ASSIGN(Hotkey);

public:
  typedef std::function<bool (int, int)> Handler;

  bool activate();
  bool deactivate();
  
  bool isActive() const { return active_; }
  
  Hotkey(unsigned fsModifiers, unsigned vk, Handler handler);
  Hotkey(Hotkey&& other) noexcept;
  ~Hotkey();
  
private:
  UINT modifiers_;
  UINT vk_;
  Handler handler_;
  bool active_;
  unsigned id_;

  static std::vector<Handler> handlers;
  static unsigned counter;
  static void setHandler(unsigned id, Handler handler);

  friend LRESULT CALLBACK HotkeyWndProc(HWND, UINT, WPARAM, LPARAM);
};

