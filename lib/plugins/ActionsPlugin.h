#pragma once

#include <vector>
#include <string>

#include <c++/stringview.h>

#include "../Plugin.h"
#include "../windows/hotkey.h"
#include <windows/controls/messagesink.h>

struct Action {
  cpp::wstring_view name;
  void (*handler)(int, int);
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

  static LRESULT onHotkey(UINT msg, WPARAM wparam, LPARAM lparam);
  static bool parseHotkey(cpp::wstring_view hotkey, std::pair<unsigned,unsigned>* result);
};

