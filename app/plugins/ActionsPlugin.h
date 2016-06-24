#pragma once

#include <vector>
#include <string>

#include <cpp-utils/strings/string_view.h>

#include <lightports/extra/hotkey.h>
#include <lightports/controls/messagesink.h>
#include "../plugin.h"

struct Action {
  cpp::wstring_view name;
  void (*handler)();
};

class ActionsPlugin : public Plugin {
public:
  ActionsPlugin();

protected:
  virtual void onActivate(const Options& options);
  virtual void onDeactivate();

private:
  std::vector<Windows::Hotkey> hotkeys_;
  Windows::MessageSink message_sink_;

  static const Action actions_[];

  static LRESULT onHotkey(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
  static bool parseHotkey(cpp::wstring_view hotkey, std::pair<unsigned,unsigned>* result);
};

