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

#ifndef HOOKLIB_HOOKMODULE_H_
#define HOOKLIB_HOOKMODULE_H_

#include <cpp-utils/pattern/registry.h>
#include <cpp-utils/strings/string_view.h>

namespace PowerWin {

class HookModuleContext
{
public:
  HookModuleContext(cpp::wstring_view name)
  : name_(name)
  { }

  // own

  cpp::wstring_view getModuleName() { return name_; }

private:
  cpp::wstring_view name_;
};

struct HookModule
{
  virtual ~HookModule() = default;
};

class HookModuleRegistryEntry
{
public:
  using factory = std::unique_ptr<HookModule>(*)(HookModuleContext&);

  explicit HookModuleRegistryEntry(
    const wchar_t* name, const wchar_t* description, const factory factory)
    : name_(name), description_(description), factory_(factory)
  { }

  template<typename V>
  static HookModuleRegistryEntry create(const wchar_t* name, const wchar_t* description)
  {
    return HookModuleRegistryEntry(name, description,
      [](HookModuleContext& context) -> std::unique_ptr<HookModule> { return std::make_unique<V>(context); });
  }

  std::unique_ptr<HookModule> create(HookModuleContext& context) const { return factory_(context); }

  const wchar_t* name() const { return name_; }
  const wchar_t* description() const { return description_; }

private:
  const wchar_t* name_;
  const wchar_t* description_;
  const factory factory_;
};


using HookModuleRegistry = cpp::registry<HookModule, HookModuleRegistryEntry>;

} // namespace PowerWin

extern template class cpp::registry<PowerWin::HookModule, PowerWin::HookModuleRegistryEntry>;

#endif /* HOOKLIB_HOOKMODULE_H_ */
