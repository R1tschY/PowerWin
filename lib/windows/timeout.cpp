#include "timeout.h"

#include <memory>

#include "debug.h"
#include "charcodecs.h"
#include "utilwindow.h"

namespace Windows {

Timeout::Timeout(const Callback& callback, int seconds):
  callback_(callback), enabled_(false)
{

  if (seconds == -1) return ;

  UINT_PTR id;
  id = SetTimer(getCallbackWindow(), (UINT_PTR)&callback_, seconds * 1000, ccallback);
  if (id != (UINT_PTR)&callback_) {
    print(L"SetTimer failed: %s\n", GetLastWindowsError().c_str());
  } else {
    enabled_ = true;
  }
}

Timeout::~Timeout() {
  if (enabled_) {
    KillTimer(getCallbackWindow(), (UINT_PTR)&callback_);
  }
}

void Timeout::setInterval(int seconds) {
  if (seconds < 0) {
    if (enabled_) {
      KillTimer(getCallbackWindow(), (UINT_PTR)&callback_);
      enabled_ = false;
    }
    return ;
  }

  UINT_PTR id;
  id = SetTimer(getCallbackWindow(), (UINT_PTR)&callback_, seconds * 1000, ccallback);
  if (id != (UINT_PTR)&callback_) {
    print(L"SetTimer failed: %s\n", Windows::GetLastWindowsError().c_str());
  } else {
    enabled_ = true;
  }
}

void Timeout::setCallback(const Callback& callback) {
  callback_ = callback;
}

void Timeout::execute(const Callback& callback, int seconds) {
  UINT_PTR id;
  UINT_PTR cb = reinterpret_cast<UINT_PTR>(new Callback(callback));

  id = SetTimer(getCallbackWindow(), cb, seconds * 1000, cexecallback);
  if (id != cb) {
    print(L"SetTimer failed: %s\n", GetLastWindowsError().c_str());
    delete reinterpret_cast<Callback*>(cb);
  }
}

void CALLBACK Timeout::ccallback(HWND, UINT msg, UINT_PTR callback, DWORD) {
  if (msg == WM_TIMER) {
    auto cb = std::unique_ptr<Callback>(reinterpret_cast<Callback*>(callback));

    if (!cb) {
       print(L"timeout callback call failed: callback == NULL\n");
       return ;
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
       return ;
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
