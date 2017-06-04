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

#pragma once

#include <cpp-utils/strings/string_view.h>
#include <cpp-utils/pattern/registry.h>
#include <cpp-utils/copy_policy.h>

#include "configuration.h"

namespace PowerWin {

// TODO: use QString

class PowerWinApp;
class Configuration;
class HotkeyManager;
class GlobalEvents;
class MouseHook;
class HookLibManager;

/// \brief context for PowerWin module
/// \details api for modules
class ModuleContext
{
public:

  ModuleContext(
    cpp::wstring_view name,
    Configuration& config,
    HotkeyManager& hotkeys,
    GlobalEvents& global_events,
    MouseHook& mouse_hook,
    HookLibManager& hook_libs
    )
  : name_(name), config_(config), hotkeys_(hotkeys),
    global_events_(global_events), mouse_hook_(mouse_hook),
    hook_libs_(hook_libs)
  { }

  // own

  cpp::wstring_view getModuleName() { return name_; }

  // global

  Configuration& getConfiguration() { return config_; }
  HotkeyManager& getHotkeyManager() { return hotkeys_; }
  GlobalEvents& getGlobalEventsManager() { return global_events_; }
  MouseHook& getMouseHook() { return mouse_hook_; }
  HookLibManager& getHookLibs() { return hook_libs_; }

private:
  cpp::wstring_view name_;
  Configuration& config_;
  HotkeyManager& hotkeys_;
  GlobalEvents& global_events_;
  MouseHook& mouse_hook_;
  HookLibManager& hook_libs_;
};

/// \brief a PowerWin module
class Module : private cpp::noncopyable {
public:
  virtual ~Module() = default;
};

/// \brief entry in PowerWin module registry
class ModuleRegistryEntry
{
public:
  using factory = std::unique_ptr<Module>(*)(ModuleContext&);

  explicit ModuleRegistryEntry(
    const wchar_t* name, const wchar_t* description, const factory factory)
    : name_(name), description_(description), factory_(factory)
  { }

  template<typename V>
  static ModuleRegistryEntry create(const wchar_t* name, const wchar_t* description)
  {
    return ModuleRegistryEntry(name, description,
      [](ModuleContext& context) -> std::unique_ptr<Module> { return std::make_unique<V>(context); });
  }

  std::unique_ptr<Module> create(ModuleContext& context) const { return factory_(context); }

  const wchar_t* name() const { return name_; }
  const wchar_t* description() const { return description_; }

private:
  const wchar_t* name_;
  const wchar_t* description_;
  const factory factory_;
};

/// \brief PowerWin module registry
using ModuleRegistry = cpp::registry<Module, ModuleRegistryEntry>;

} // namespace PowerWin

extern template class cpp::registry<PowerWin::Module, PowerWin::ModuleRegistryEntry>;

