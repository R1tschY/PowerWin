#include "Timeout.h"
#include "../DesktopHooks.h"
#include "../Utils.h"
#include "../windows/debug.h"
#include "../windows/charcodecs.h"

////////////////////////////////////////////////////////////////////////////////
//     Timeout
////////////////////////////////////////////////////////////////////////////////
Timeout::Timeout(const Callback& callback, int seconds):
  callback_(callback), enabled_(false) {

  if (seconds == -1) return ;

  UINT_PTR id;
  id = SetTimer(WinExtra::getMainWindow(), (UINT_PTR)&callback_, seconds * 1000, ccallback);
  if (id != (UINT_PTR)&callback_) {
    print(L"! Timeout-Timer-Erstellung scheitert: %s\n", Windows::GetLastWindowsError().c_str());
  } else {
    enabled_ = true;
  }
}

Timeout::~Timeout() {
  if (enabled_) {
    KillTimer(WinExtra::getMainWindow(), (UINT_PTR)&callback_);
  }
}

void Timeout::setInterval(int seconds) {
  if (seconds < 0) {
    if (enabled_) {
      KillTimer(WinExtra::getMainWindow(), (UINT_PTR)&callback_);
      enabled_ = false;
    }
    return ;
  }

  UINT_PTR id;
  id = SetTimer(WinExtra::getMainWindow(), (UINT_PTR)&callback_, seconds * 1000, ccallback);
  if (id != (UINT_PTR)&callback_) {
    print(L"! Timeout-Timer-Erstellung scheitert: %s\n", Windows::GetLastWindowsError().c_str());
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

  id = SetTimer(WinExtra::getMainWindow(), cb, seconds * 1000, cexecallback);
  if (id != cb) {
    print(L"! Timeout-Timer-Erstellung scheitert: %s\n", Windows::GetLastWindowsError().c_str());
    delete reinterpret_cast<Callback*>(cb);
  }
}

void CALLBACK Timeout::ccallback(HWND hwnd, UINT msg, UINT_PTR callback, DWORD time) {
  if (msg == WM_TIMER) {
    Callback* cb = reinterpret_cast<Callback*>(callback);

    if (cb == nullptr) {
       OutputDebugString(L"Timeout Funktionsaufruf scheitert: callback == NULL\n");
       return ;
    }

    try {
      (*cb)();
    } catch (const boost::bad_function_call& error) {
      OutputDebugString(L"! Timeout Funktionsaufruf scheitert:");
      OutputDebugString(Windows::convertFromUtf8(error.what()).c_str());
      OutputDebugString(L"\n");
    }
  }
}

void CALLBACK Timeout::cexecallback(HWND hwnd, UINT msg, UINT_PTR callback, DWORD time) {
  if (msg == WM_TIMER) {
    Callback* cb = reinterpret_cast<Callback*>(callback);

    if (cb == NULL) {
       OutputDebugString(L"Timeout Funktionsaufruf scheitert: callback == NULL\n");
       return ;
    }

    try {
      (*cb)();
    } catch (const boost::bad_function_call& error) {
      OutputDebugString(L"! Timeout Funktionsaufruf scheitert: %s\n");
      OutputDebugString(Windows::convertFromUtf8(error.what()).c_str());
      OutputDebugString(L"\n");
    }

    KillTimer(WinExtra::getMainWindow(), callback);
    delete cb;
  }
}

