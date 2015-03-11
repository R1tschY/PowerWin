#include "timeout.h"

#include <memory>

#include "debug.h"
#include "charcodecs.h"
#include "utilwindow.h"

namespace Windows {

Timeout::Timeout(const Callback& callback, int seconds):
  callback_(callback), enabled_(false)
{
  if (seconds == -1) return;

  win_throw_on_fail(SetTimer(getCallbackWindow(), (UINT_PTR)&callback_, seconds * 1000, ccallback));
  enabled_ = true;
}

Timeout::~Timeout() {
  if (enabled_) {
    KillTimer(getCallbackWindow(), (UINT_PTR)&callback_);
  }
}

void Timeout::setInterval(int seconds) {
  if (seconds <= 0) {
    if (enabled_) {
      KillTimer(getCallbackWindow(), (UINT_PTR)&callback_);
      enabled_ = false;
    }
    return;
  }

  win_throw_on_fail(
        SetTimer(getCallbackWindow(), (UINT_PTR)&callback_, seconds * 1000, ccallback));
  enabled_ = true;
}

void Timeout::setCallback(const Callback& callback) {
  callback_ = callback;
}

void Timeout::execute(const Callback& callback, int seconds) {
  UINT_PTR cb = reinterpret_cast<UINT_PTR>(new Callback(callback));
  win_throw_on_fail(SetTimer(getCallbackWindow(), cb, seconds * 1000, cexecallback));
}

void CALLBACK Timeout::ccallback(HWND, UINT msg, UINT_PTR callback, DWORD) {
  if (msg == WM_TIMER) {
    auto cb = std::unique_ptr<Callback>(reinterpret_cast<Callback*>(callback));

    if (!cb) {
       print(L"timeout callback call failed: callback == NULL\n");
       return;
    }

    try {
      (*cb)();
    } catch (const std::bad_function_call& error) {
      print(L"timeout callback call failed: %s\n",
            to_wstring(error.what()).c_str());
    }
  }
}

void CALLBACK Timeout::cexecallback(HWND, UINT msg, UINT_PTR callback, DWORD) {
  if (msg == WM_TIMER) {
    auto cb = std::unique_ptr<Callback>(reinterpret_cast<Callback*>(callback));

    if (!cb) {
       print(L"timeout callback call failed: callback == NULL\n");
       return;
    }

    try {
      (*cb)();
    } catch (const std::bad_function_call& error) {
      print(L"timeout callback call failed: %s\n",
            to_wstring(error.what()).c_str());
    }

    KillTimer(getCallbackWindow(), callback);
  }
}

} // namespace Windows
