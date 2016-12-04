///
/// Copyright (c) 2016 R1tschY
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to 
/// deal in the Software without restriction, including without limitation the 
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#include "modulemanager.h"

#include <functional>
#include <cpp-utils/algorithm/container.h>
#include <lightports/core/debugstream.h>

#include "mousehook.h"

namespace PowerWin {

void ManagedModule::activate()
{
  if (module_) return;

  ModuleContext context(entry_.name(), mgr_.getConfiguration(),
      mgr_.getHotkeys(), mgr_.getGlobalEvents(), mgr_.getMouseHook());
  module_ = entry_.create(context);
}

void ManagedModule::deactivate()
{
  module_.reset();
}

ModuleManager::ModuleManager(Configuration& configuration,
    HotkeyManager& hotkeys, GlobalEvents& global_events)
: config_(configuration), hotkeys_(hotkeys), global_events_(global_events),
  mouse_hook_(MouseHook::getInstance())
{ }

ModuleManager::~ModuleManager()
{ }

void ModuleManager::loadModules()
{
  // TODO: use a cool back_emplacer: ModuleRegistry::entries() | filter() | transform([](auto& entry){ return make_tuple(*this, entry); }) | back_emplace(modules);
  for (auto& entry : ModuleRegistry::entries())
  {
    modules_.emplace_back(*this, entry);

    if (config_.readBoolean(entry.name(), L"active", true))
    {
      Windows::DebugOutputStream() << L"activated module " << entry.name();
      modules_.back().activate();
    }
  }
  mouse_hook_.activate();
}

void ModuleManager::unloadModules()
{
  mouse_hook_.deactivate();
  modules_.clear();
}

} // namespace PowerWin
