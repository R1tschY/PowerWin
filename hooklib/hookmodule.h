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

namespace PowerWin {

class HookModuleContext
{
  void registerRPCFunction();
};

struct HookModule
{
  virtual ~HookModule() = default;

  virtual void init(HookModuleContext& context) = 0;

  virtual void activate() = 0;
  virtual void deactivate() = 0;
};

using HookModuleRegistry = cpp::registry<HookModule>;

} // namespace PowerWin

extern template class cpp::registry<PowerWin::HookModule>;

#endif /* HOOKLIB_HOOKMODULE_H_ */
