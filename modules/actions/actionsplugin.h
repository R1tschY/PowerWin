#pragma once

#include <vector>
#include <string>

#include <cpp-utils/strings/string_view.h>

#include <lightports/user/hotkey.h>
#include <lightports/controls/messagesink.h>
#include "../../app/module.h"

class ActionsPlugin : public PowerWin::Module {
public:
  ActionsPlugin(PowerWin::ModuleContext& context);
  ~ActionsPlugin();
};

