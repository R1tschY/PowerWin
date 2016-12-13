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

#ifndef HOOKLIB_SYSTEMMENUHOOK_H_
#define HOOKLIB_SYSTEMMENUHOOK_H_

#include <cpp-utils/storage/uninitized.h>
#include <lightports/user/hook.h>

#include <hooklib/hookmodule.h>

class SystemMenuHook
{
public:
  enum {
    AliveTag = 0x12345678,
    DeadTag = 0xDEADDEAD
  };

  SystemMenuHook(PowerWin::HookModuleContext& context);
  ~SystemMenuHook();

  void onNewWindow(HWND hwnd);
  void onToogleTopmost(HWND hwnd);

  static LRESULT CALLBACK onHookMessage(int code, WPARAM wParam, LPARAM lParam);

  bool isAlive() const { return alive_ == AliveTag; }

private:
  std::uint32_t alive_;
  UINT new_window_msg_;
  UINT set_topmost_msg_;
  HWND powerwin_hwnd_;
  Windows::Hook hook_;
};

class SystemMenuHookModule : public PowerWin::HookModule
{
public:
  SystemMenuHookModule(PowerWin::HookModuleContext& context);

private:
  cpp::scoped_lifetime<SystemMenuHook> hook_;
};

#endif /* HOOKLIB_SYSTEMMENUHOOK_H_ */
