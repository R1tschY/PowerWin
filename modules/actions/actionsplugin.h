#pragma once

#include <vector>
#include <string>

#include <cpp-utils/strings/string_view.h>

#include <lightports/extra/hotkey.h>
#include <lightports/controls/messagesink.h>
#include "../../app/module.h"

struct Action {
  cpp::wstring_view name;
  void (*handler)();
};

class ActionsPlugin : public PowerWin::Module {
public:
  ActionsPlugin();

protected:
  void activate(PowerWin::ModuleContext& context) override;
  void deactivate() override;

private:
  static bool parseHotkey(cpp::wstring_view hotkey, std::pair<unsigned,unsigned>* result);
};

