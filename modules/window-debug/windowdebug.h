#pragma once

#include <lightports/controls/onscreendisplay.h>
#include <lightports/extra/hook.h>
#include <lightports/base/timeout.h>
#include <app/module.h>
#include <app/hotkeymanager.h>

namespace PowerWin {

class WindowDebugView : public Windows::OnScreenDisplay
{
public:
  WindowDebugView();

  void onPaint(Gdiplus::Graphics& context) override;
  void onCreate() override;

private:
  Windows::Timeout refresh_timeout_;
};

class WindowDebugModule : public Module {
public:
  WindowDebugModule(ModuleContext& context);
  ~WindowDebugModule();

  void showView();
  void hideView();

private:
  WindowDebugView debug_view_;
  Hotkey show_hotkey_;
  Hotkey hide_hotkey_;
};

} // namespace PowerWin

