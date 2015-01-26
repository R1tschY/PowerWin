#pragma once

#include <functional>

#include "macros.h"
#include <windows.h>

namespace Windows {

////////////////////////////////////////////////////////////////////////////////
//     Timeout
////////////////////////////////////////////////////////////////////////////////
class Timeout {
  DISALLOW_COPY_AND_ASSIGN(Timeout);

public:
  typedef std::function<void()> Callback;

  Timeout(const Callback& callback, int seconds);
  ~Timeout();
  void setInterval(int seconds);
  void setCallback(const Callback& callback);

  static void execute(const Callback& callback, int seconds);

private:
  Callback callback_;
  bool enabled_; 

  static void CALLBACK ccallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
  static void CALLBACK cexecallback(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
};

} // namespace Windows

