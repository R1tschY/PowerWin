#include <utility>

#include <cpp-utils/strings/string_literal.h>
#include <cpp-utils/algorithm/length.h>
#include <lightports/core/debug.h>
#include <app/powerwinapp.h>
#include <modules/actions/actionsplugin.h>


ActionsPlugin::ActionsPlugin()
{ }

void ActionsPlugin::activate(PowerWin::ModuleContext& context)
{
//  std::pair<unsigned,unsigned> hotkey;
//  int index = 0;
//  for (const Action& action : actions_) {
//    auto option = options.find(std::wstring(action.name));
//    if (option != options.end() &&
//        parseHotkey(option->second, &hotkey))
//    {
//      Windows::Hotkey hk(
//    		  hotkey.first, hotkey.second,
//    		  message_sink_.getNativeHandle(),
//			  index);
//      hk.activate();
//      hotkeys_.emplace_back(std::move(hk));
//    }
//    index++;
//  }
}

void ActionsPlugin::deactivate() {
//  for (auto& hotkey : hotkeys_)
//    hotkey.deactivate();
//
//  message_sink_.destroy();
}

PowerWin::ModuleRegistry::element<ActionsPlugin> ActionsModule(
  L"actions", L"configure hotkeys for windows"
);
