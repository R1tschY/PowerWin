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

#include <cstddef>

#include <cpp-utils/storage/uninitized.h>
#include <lightports/user/geometry.h>
#include <lightports/user/cursor.h>
#include <lightports/user/hook.h>
#include <hooklib/hookmodule.h>
#include <hooklib/macros.h>

namespace PowerWin {

class WKillHook
{
public:
  enum {
    AliveTag = 0x12345678,
    DeadTag = 0xDEADDEAD
  };

  WKillHook();
  ~WKillHook();

  static LRESULT onHookMessage(Windows::Hook& hook, int code, WPARAM wParam, LPARAM lParam);
  static Windows::Hook& getHook();

  bool isAlive() const { return alive_ == AliveTag; }

private:
  std::uint32_t alive_;
  Windows::Hook hook_;
  Windows::CursorHandle cursor32_;
  Windows::CursorHandle cursor64_;
  int count_ = 0;
  int count2_ = 0;

  friend class WKillHookModule;
};

class WKillHookModule : public PowerWin::HookModule
{
public:
  WKillHookModule(HookModuleContext& context);

  cpp::optional<LRESULT> processMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
  std::unique_ptr<cpp::scoped_lifetime<WKillHook>> hook_;
  UINT mouse_control_msg_ = 0;
};

} // namespace PowerWin
