#ifndef SYSTEMINFORMATION_H
#define SYSTEMINFORMATION_H

#include <windows.h>

#include "geometry.h"
#include <c++/utils.h>

namespace Windows {

class Monitor {
public:
  // types
  class Info {
  public:
    // ctor
    Info() : info_{sizeof(info_)} { }
    Info(const MONITORINFO& info) : info_(info) { }

    // native
    MONITORINFO& getNativeRef() {
      return info_;
    }

    // properties
    DWORD getSize() const { return info_.cbSize; }
    const Rectangle& getMonitorRect() const { return cpp::wrap<Rectangle>(info_.rcMonitor); }
    const Rectangle& getWorkRect() const { return cpp::wrap<Rectangle>(info_.rcWork); }
    bool isPrimary() const { return info_.dwFlags == MONITORINFOF_PRIMARY; }

  private:
    MONITORINFO info_;
  };

  // ctors
  Monitor(HMONITOR handle) :
    handle_(handle)
  { }

  static Monitor getPrimary() {
    return Monitor(MonitorFromPoint(POINT{0,0}, MONITOR_DEFAULTTOPRIMARY));
  }

  // properties
  HMONITOR getNativeHandle() { return handle_; }

  Info getInfo() {
    Info info;
    GetMonitorInfo(handle_, &info.getNativeRef());
    return info;
  }

private:
  HMONITOR handle_;
};

} // namespace Windows

#endif // SYSTEMINFORMATION_H