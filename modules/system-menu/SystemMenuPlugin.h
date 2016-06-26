#ifndef SYSTEMMENU_H
#define SYSTEMMENU_H

#include <app/plugin.h>
#include <lightports/ipc/ipcconnection.h>

class SystemMenuPlugin : public Plugin {
public:
  SystemMenuPlugin();

private:
  virtual void onActivate(const Options& options);
  virtual void onDeactivate();

private:
  // TODO: use a application wide context
  Windows::IPCConnection connection32_;
  Windows::IPCConnection connection64_;
};

#endif // SYSTEMMENU_H
