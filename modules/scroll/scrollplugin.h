#pragma once

#include <lightports/extra/hook.h>
#include <app/module.h>

constexpr int SHIFTED = 0x8000;

class ScrollPlugin : public PowerWin::Module {
public:
  ScrollPlugin(PowerWin::ModuleContext& context);
  ~ScrollPlugin();

private:
  Windows::Hook hook_;
  bool inverse_;

  static LRESULT CALLBACK hookProc(int code, WPARAM wparam, LPARAM lparam);
  bool handle(POINT pt, int steps);
};

