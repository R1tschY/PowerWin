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

class ManagedModule
{
public:
  template<typename T>
  ManagedModule(ModuleManager& mgr, const T& t)
  : mgr_(mgr), module_(t.create()), name_(t.name())
  { }

  void activate();
  void deactivate();

  bool isOkay() { return module_ != nullptr; }

  Module* getModule() { return module_.get(); }
  cpp::wstring_view getName() const { return name_; }
  bool isActive() const { return active_; }

private:
  ModuleManager& mgr_;

  std::unique_ptr<Module> module_;
  std::wstring name_;
  bool active_ = false;
};

/// \brief
class ModuleManager
{
public:
  ModuleManager(Configuration& configuration);
  ~ModuleManager();

  Configuration& getConfiguration() { return config_; }

  void loadModules();
  void unloadModules();

private:
  std::vector<ManagedModule> modules_;
  Configuration& config_;
};

} // namespace PowerWin

#endif /* APP_MODULEMANAGER_H_ */
