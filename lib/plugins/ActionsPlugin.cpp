#include "ActionsPlugin.h"

#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/lexical_cast.hpp>

#include "../DesktopHooks.h"
#include "../windows/debug.h"

ActionsPlugin::ActionsPlugin() : Plugin(lit("actions")) { }

static bool onActionQuit(int, int) {
  win_destroy();
  return true;
}

Action ActionsPlugin::actions_[] = {
  {L"quit", onActionQuit}  
};

void ActionsPlugin::onActivate(const Options& options) {
  std::pair<unsigned,unsigned> hotkey;

  if (options.find(L"quit") == options.end()) {
    parseHotkey(L"Ctrl+F12", &hotkey);
    Hotkey hk(hotkey.first, hotkey.second, onActionQuit);
    hk.activate();
    hotkeys_.emplace_back(std::move(hk));
  }

  for (const Action& action : actions_) {
    auto option = options.find(action.name);
    if (option != options.end() &&
        parseHotkey(option->second, &hotkey))
    {
      Hotkey hk(hotkey.first, hotkey.second, action.handler);
      hk.activate();
      hotkeys_.emplace_back(std::move(hk));
    }
  }
}

void ActionsPlugin::onDeactivate() {
  for (auto& hotkey : hotkeys_)
    hotkey.deactivate();
}

int get_function_key(const std::wstring& key) {
  int fkey;
  try {
    fkey = boost::lexical_cast<int>(key.c_str() + 1);
  } catch (const boost::bad_lexical_cast&) {
    return 0;
  }

  return (fkey > 0 && fkey < 25) ? fkey - 1 + VK_F1 : 0;
}

bool
ActionsPlugin::parseHotkey(std::wstring hotkey, std::pair<unsigned,unsigned>* result) {
  std::vector<std::wstring> splited;
  boost::split(splited, hotkey, boost::is_any_of(L" +"), boost::token_compress_on);

  UINT modifiers = 0;
  UINT vk = 0;

  for (std::wstring& key : splited) {
    boost::to_upper(key);

    if (key.size() == 1) {
      if ((key[0] >= L'A' && key[0] <= L'Z') ||
          (key[0] >= L'0' && key[0] <= L'9'))
      {
        vk = key[0];
        continue;
      } else {
        return false;
      }
    }

    if (key[0] == 'F') {
      int fkey = get_function_key(key);
      if (fkey != 0) {
        vk = fkey;
        continue;
      } else {
        return false;
      }
    }
    if (key.compare(L"CTRL") == 0) {
      modifiers |= MOD_CONTROL;
    } else if (key.compare(L"ALT") == 0) {
      modifiers |= MOD_ALT;
    } else if (key.compare(L"WIN") == 0) {
      modifiers |= MOD_WIN;
    } else if (key.compare(L"SHIFT") == 0) {
      modifiers |= MOD_SHIFT;
    } else {
      return false;
    }
  }

  *result = std::make_pair(modifiers, vk);
  return true;
}
