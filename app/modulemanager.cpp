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

namespace PowerWin {

void ManagedModule::activate()
{
  if (active_) return;

  ModuleContext context(name_, mgr_.getConfiguration(), mgr_.getHotkeys());
  module_->activate(context);
}

void ManagedModule::deactivate()
{
  if (!active_) return;

  module_->deactivate();
}

ModuleManager::ModuleManager(Configuration& configuration, HotkeyManager& hotkeys)
: config_(configuration), hotkeys_(hotkeys)
{ }

ModuleManager::~ModuleManager()
{ }

void ModuleManager::loadModules()
{
  // TODO: use a cool back_emplacer: ModuleRegistry::entries() | filter() | transform([](auto& entry){ return make_tuple(*this, entry); }) | back_emplace(modules);
  for (auto& entry : ModuleRegistry::entries())
  {
    if (config_.readBoolean(entry.name(), L"active", true))
    {
      modules_.emplace_back(*this, entry);
      modules_.back().activate();
    }
  }
}

void ModuleManager::unloadModules()
{
  // TODO: idea: fn cpp::member_call(MemFunc&f,Args...args) { std::bind(std::men_fn(f), args...) }
  cpp::for_each(modules_, std::mem_fn(&ManagedModule::deactivate));
}

} // namespace PowerWin
