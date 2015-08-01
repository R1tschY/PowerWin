#pragma once

#include "../plugin.h"
#include <windows/extra/hook.h>
#include <windows/base/timeout.h>

class FullscreenPlugin : public Plugin {
public:
  FullscreenPlugin();

  virtual void onActivate(const Options& options);
  virtual void onDeactivate();

private:
  Windows::Timeout timeout_;
  bool fullscreen_window_;

  void onTimeout();
};

