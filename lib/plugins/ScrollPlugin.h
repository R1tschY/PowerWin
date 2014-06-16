#pragma once

#include "../stdafx.h"
#include "../Plugin.h"
#include "../Hook.h"

constexpr int SHIFTED = 0x8000;

class ScrollPlugin : public Plugin {
public:
  ScrollPlugin();

  virtual void onActivate(const Options& options);
  virtual void onDeactivate();

private:
  MouseWheelHook hook_;
  bool inverse_;

  bool handle(POINT pt, int steps);
};

