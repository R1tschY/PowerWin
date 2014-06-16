#pragma once

#include "Stdafx.h"

#include <boost/function.hpp>
#include <vector>
#include <cassert>
#include <cstdio>

//////////////////////////////////////////////////////////////////////////////
// Hook

class Hook {
public:
  static const DWORD THREAD_ID_ALL_THREADS = 0;

  bool activate();
  bool deactivate();
  
  bool isActive() const { return hook_ != NULL; }
  HHOOK getHandle() const { return hook_; }
  
  Hook(int hook_id, DWORD thread_id, HOOKPROC func) :
    hook_id_(hook_id), thread_id_(thread_id), func_(func), hook_(NULL)
  { }
  
private:
  int hook_id_;
  DWORD thread_id_;
  HOOKPROC func_;
  HHOOK hook_;
};

///////////////////////////////////////////////////////////////////////////////
// MouseHook

class MouseHook : public Hook {
public:
  typedef boost::function<bool (POINT, int)> WheelHandler;
  typedef boost::function<bool (unsigned, POINT)> ClickHandler;
  
  static MouseHook& get();
  
  bool empty() const {
    return wheel_hooks_.empty() && click_hooks_.empty(); // && *.empty();
  }

  MouseHook() :
    Hook(WH_MOUSE_LL, THREAD_ID_ALL_THREADS, hookProc),
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
