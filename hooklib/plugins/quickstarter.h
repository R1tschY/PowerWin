/// \file quickstarter.h

#ifndef HOOKLIB_PLUGINS_QUICKSTARTER_H_
#define HOOKLIB_PLUGINS_QUICKSTARTER_H_

#include "../plugin.h"
#include "windows/ipc/ipcmailbox.h"
#include "windows/ipc/ipcconnection.h"
#include <thread>

class QuickStarter : public Plugin
{
public:
  QuickStarter();

  void updateDatabase();
  void show();

private:
  void onActivate(const Options& options) override;
  void onDeactivate() override;

  void updateApplicationDb();

private:
  // TODO: sqlite3* handle_
  Windows::IPCMailbox mailslot_;
  Windows::IPCConnection connection_;

  std::thread thread_;
};

#endif /* HOOKLIB_PLUGINS_QUICKSTARTER_H_ */
