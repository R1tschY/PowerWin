#pragma once

#include "../stdafx.h"

#include "../Plugin.h"
#include "../Hook.h"
#include "../Timeout.h"

class FullscreenPlugin : public Plugin {
public:
  FullscreenPlugin();

  virtual void onActivate(const Options& options);
  virtual void onDeactivate();

private:
  Timeout timeout_;
  bool fullscreen_window_;

  void onTimeout();
};

