#pragma once

#include <windows.h>
#include <functional>
#include <vector>
#include <cassert>
#include <cstdio>

namespace Windows {

//////////////////////////////////////////////////////////////////////////////
// Hook

class Hook {
public:
  static const DWORD AllThreads = 0;

  bool create(int hook_id, DWORD thread_id, HOOKPROC func);
  bool destroy();

  bool isActive() const { return hook_ != nullptr; }
  HHOOK getHandle() const { return hook_; }

  Hook() : hook_(nullptr) { }

  LRESULT callNext(int code, WPARAM wparam, LPARAM lparam) const {
    return CallNextHookEx(hook_, code, wparam, lparam);
  }

private:
  HHOOK hook_;
};

///////////////////////////////////////////////////////////////////////////////
// MouseHook

class MouseHook {
public:
  typedef std::function<bool (POINT, int)> WheelHandler;
  typedef std::function<bool (unsigned, POINT)> ClickHandler;

  static MouseHook& get();

  bool empty() const {
    return wheel_hooks_.empty() && click_hooks_.empty(); // && *.empty();
  }

  MouseHook() :
    hook_(),
    wheel_hooks_()
  { }

protected:
  template<typename Proc>
  friend class MouseHookProxy;

  void addHandler(WheelHandler* proc);
  void addHandler(ClickHandler* proc);
  void removeHandler(WheelHandler* proc);
  void removeHandler(ClickHandler* proc);

private:
  Hook hook_;
  std::vector<WheelHandler*> wheel_hooks_;
  std::vector<ClickHandler*> click_hooks_;

  static LRESULT CALLBACK hookProc(int code, WPARAM wparam, LPARAM lparam);
};

///////////////////////////////////////////////////////////////////////////////
// MouseHookProxy

template<typename Proc>
class MouseHookProxy {
public:
  explicit MouseHookProxy(Proc proc) :
    proc_(proc), active_(false)
  { }

  ~MouseHookProxy() {
    deactivate();
  }

  void activate();
  void deactivate();

  bool isActive() const {
    return active_;
  }

private:
  Proc proc_;
  bool active_;
};

template<typename Proc>
void MouseHookProxy<Proc>::activate() {
  if (!active_) {
    MouseHook::get().addHandler(&proc_);
    active_ = true;
  }
}

template<typename Proc>
void MouseHookProxy<Proc>::deactivate(){
  if (active_) {
    MouseHook::get().removeHandler(&proc_);
    active_ = false;
  }
}

typedef MouseHookProxy<MouseHook::WheelHandler> MouseWheelHook;
typedef MouseHookProxy<MouseHook::ClickHandler> MouseClickHook;

} // namespace Windows


