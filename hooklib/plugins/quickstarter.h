/// \file quickstarter.h

#ifndef HOOKLIB_PLUGINS_QUICKSTARTER_H_
#define HOOKLIB_PLUGINS_QUICKSTARTER_H_

#include "../plugin.h"

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
};

#endif /* HOOKLIB_PLUGINS_QUICKSTARTER_H_ */
