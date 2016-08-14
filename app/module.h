#pragma once

#include <cpp-utils/strings/string_view.h>
#include <cpp-utils/pattern/registry.h>
#include <cpp-utils/copy_policy.h>

#include "configuration.h"

namespace PowerWin {

class PowerWinApp;
class Configuration;
class HotkeyManager;

class ModuleContext
{
public:

  ModuleContext(
    cpp::wstring_view name,
    Configuration& config,
    HotkeyManager& hotkeys
    )
  : name_(name), config_(config), hotkeys_(hotkeys)
  { }

  // own

  cpp::wstring_view getModuleName() { return name_; }

  // global

  Configuration& getConfiguration() { return config_; }
  HotkeyManager& getHotkeyManager() { return hotkeys_; }

private:
  cpp::wstring_view name_;
  Configuration& config_;
  HotkeyManager& hotkeys_;
};

class Module : private cpp::noncopyable {
public:
  virtual ~Module() = default;
};

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

using ModuleRegistry = cpp::registry<Module, ModuleRegistryEntry>;

} // namespace PowerWin

extern template class cpp::registry<PowerWin::Module, PowerWin::ModuleRegistryEntry>;

