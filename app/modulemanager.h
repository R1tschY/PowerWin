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

#ifndef APP_MODULEMANAGER_H_
#define APP_MODULEMANAGER_H_

#include "module.h"

namespace PowerWin {

class PowerWinApp;
class Configuration;
class ModuleManager;
class GlobalEvents;

class ManagedModule
{
public:
  ManagedModule(ModuleManager& mgr, const ModuleRegistryEntry& entry)
  : mgr_(mgr), entry_(entry)
  { }

  void activate();
  void deactivate();

  Module* getModule() { return module_.get(); }
  cpp::wstring_view getName() const { return entry_.name(); }
  bool isActive() const { return bool(module_); }

private:
  ModuleManager& mgr_;
  const ModuleRegistryEntry& entry_;

  std::unique_ptr<Module> module_ = {};
};

/// \brief
class ModuleManager
{
public:
  ModuleManager(Configuration& configuration, HotkeyManager& hotkeys,
      GlobalEvents& global_events, HookLibManager& hook_libs);
  ~ModuleManager();

  Configuration& getConfiguration() { return config_; }
  HotkeyManager& getHotkeys() { return hotkeys_; }
  GlobalEvents& getGlobalEvents() { return global_events_; }
  MouseHook& getMouseHook() { return mouse_hook_; }
  HookLibManager& getHookLibs() { return hook_libs_; }

  void loadModules();
  void unloadModules();

private:
  std::vector<ManagedModule> modules_;
  Configuration& config_;
  HotkeyManager& hotkeys_;
  GlobalEvents& global_events_;
  MouseHook& mouse_hook_;
  HookLibManager& hook_libs_;
};

} // namespace PowerWin

#endif /* APP_MODULEMANAGER_H_ */
