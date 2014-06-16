#pragma once

#include "stdafx.h"

#include <functional>
#include <vector>
#include <boost/noncopyable.hpp>

#include "extra.h"

class Hotkey : boost::noncopyable {
public:
  typedef std::function<bool (int, int)> Handler;

  bool activate();
  bool deactivate();
  
  bool isActive() const { return active_; }
  
  Hotkey(UINT fsModifiers, UINT vk, Handler handler);
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

