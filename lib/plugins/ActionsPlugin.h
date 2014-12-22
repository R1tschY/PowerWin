#pragma once

#include <vector>
#include <string>

#include <c++/stringview.h>

#include "../Plugin.h"
#include "../windows/hotkey.h"

struct Action {
  cpp::wstring_view name;
  bool (*handler)(int, int);
};

class ActionsPlugin : public Plugin {
public:
  ActionsPlugin();

protected:
  virtual void onActivate(const Options& options);
  virtual void onDeactivate();

private:
  std::vector<Hotkey> hotkeys_;
  static Action actions_[];

  static bool parseHotkey(std::wstring_view hotkey, std::pair<unsigned,unsigned>* result);
};

