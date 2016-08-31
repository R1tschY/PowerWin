#ifndef PLUGINS_SPLASHSCREENPLUGIN_H_
#define PLUGINS_SPLASHSCREENPLUGIN_H_

#include <windows.h>

#include <app/module.h>
#include <app/helper/observermixin.h>
#include <app/globalevents.h>

class SystemMenuModule : public PowerWin::Module, private PowerWin::ObserverMixin
{
public:
  SystemMenuModule(PowerWin::ModuleContext& context);
  ~SystemMenuModule();

private:
  UINT new_window_msg_;
  UINT toggle_topmost_msg_;

  void onGlobalMessage(PowerWin::WindowsMessageEvent& e);
  void onNewWindow(HWND hwnd);
  void onToogleTopmost(HWND hwnd);
};

#endif /* PLUGINS_SPLASHSCREENPLUGIN_H_ */
