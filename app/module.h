#pragma once

#include <cpp-utils/strings/string_view.h>
#include <cpp-utils/pattern/registry.h>
#include <cpp-utils/copy_policy.h>

#include "configuration.h"

namespace PowerWin {

class PowerWinApp;
class Configuration;

class ModuleContext
{
public:

  ModuleContext(
    cpp::wstring_view name,
    Configuration& config
    )
  : name_(name), config_(config)
  { }

  // own

  cpp::wstring_view getModuleName() { return name_; }

  // global

  Configuration& getConfiguration() { return config_; }

private:
  cpp::wstring_view name_;
  Configuration& config_;
};

class Module : private cpp::noncopyable {
public:
  virtual ~Module() = default;

  virtual void activate(ModuleContext& context) = 0;
  virtual void deactivate() = 0;
};

using ModuleRegistry = cpp::registry<Module, cpp::simple_registry_entry<Module, wchar_t>>;

} // namespace PowerWin

extern template class cpp::registry<PowerWin::Module, cpp::simple_registry_entry<PowerWin::Module, wchar_t>>;

