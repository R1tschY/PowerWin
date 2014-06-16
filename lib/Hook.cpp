#include "StdAfx.h"

#include "algorithm.h"
#include "extra.h"

#include "Hook.h"
#include "DesktopHooks.h"

///////////////////////////////////////////////////////////////////////////////
// Hook

bool Hook::activate() {
  if (!hook_) {
    hook_ = SetWindowsHookEx(hook_id_, func_, WinExtra::getDllInstance(), thread_id_);
    if (hook_ == NULL)  MessageBeep(MB_ICONERROR);
    return (hook_ != NULL);
  } else {
    return true;
  }
}

bool Hook::deactivate() {
  if (hook_ && UnhookWindowsHookEx(hook_)) {
    hook_ = NULL;
    return true;
  } else {
    return (hook_ == NULL);
  }
}

///////////////////////////////////////////////////////////////////////////////
// MouseHook
static MouseHook instance DLL_SHARED;

MouseHook& MouseHook::get() {
  return instance;
}

void MouseHook::addHandler(MouseHook::WheelHandler* proc) {
  wheel_hooks_.push_back(proc);
  activate();
}

void MouseHook::removeHandler(MouseHook::WheelHandler* proc) {
  extra::remove(wheel_hooks_, proc);
  if (empty()) {
    if (!deactivate()) {
      MessageBeep(MB_ICONERROR);
    }
  }
}

void MouseHook::addHandler(MouseHook::ClickHandler* proc) {
  click_hooks_.push_back(proc);
  activate();
}

void MouseHook::removeHandler(MouseHook::ClickHandler* proc) {
  extra::remove(click_hooks_, proc);
  if (empty()) {
    if (!deactivate()) {
      MessageBeep(MB_ICONERROR);
    }
  }
}

__attribute__((hot))
LRESULT CALLBACK MouseHook::hookProc(int code, WPARAM wparam, LPARAM lparam) {
  if (code != HC_ACTION) {
    return CallNextHookEx(get().getHandle(), code, wparam, lparam);
  }
    
  bool processed = false;
  MSLLHOOKSTRUCT* data = reinterpret_cast<MSLLHOOKSTRUCT*>(lparam);

  switch (wparam) {
  case WM_MOUSEWHEEL: 
    for (auto& hook : get().wheel_hooks_) {
      if ((*hook)(data->pt, HIWORD(data->mouseData))) processed = true;
    }
    break;
                     
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
    for (auto& hook : get().click_hooks_) {
      if ((*hook)(wparam, data->pt)) processed = true;
    }
    break;
  }
    
  if (!processed) {
    return CallNextHookEx(instance.getHandle(), code, wparam, lparam);
  } else {
    return 1;
  }
}
