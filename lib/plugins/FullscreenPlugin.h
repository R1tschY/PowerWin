#pragma once

#include "../Plugin.h"
#include "../windows/hook.h"
#include "../windows/timeout.h"

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

