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

#include "wkillhook.h"

#include <windows.h>
#include <cpp-utils/storage/uninitized.h>
#include <lightports/dll/dll.h>
#include <lightports/core.h>
#include <lightports/os/process.h>
#include <lightports/controls/control.h>
#include <lightports/core/debugstream.h>
#include <hooklib/macros.h>

#include "common.h"


using namespace Windows;

namespace PowerWin {

using namespace Windows;

WINDOWS_DLL_SHARED cpp::uninitialized<WKillHook> wkillhook;

WKillHook::WKillHook()
{
  cpp_assert(!isAlive());
  alive_ = AliveTag;

  hook_.create(
      Dll::getHINSTANCE(),
      Hook::Mouse, Hook::AllThreads,
      &hookProc<&WKillHook::getHook, &WKillHook::onHookMessage>);

  cursor64_ = loadCursor(StockCursor::Cross).clone();
}

WKillHook::~WKillHook()
{
  alive_ = DeadTag;
}

Hook& WKillHook::getHook() { return wkillhook->hook_; }

LRESULT WKillHook::onHookMessage(Hook& hook, int code, WPARAM wParam,
    LPARAM lParam)
{
  UINT msg = (UINT)wParam;
  MOUSEHOOKSTRUCT* infos = (MOUSEHOOKSTRUCT*)lParam;

#if CPUBITSET == 64
  // set cursor
  setCursor(wkillhook->cursor64_);

  return 1;
#endif

  // prevent message to window
  return hook.callNext(code, wParam, lParam);
}

WKillHookModule::WKillHookModule(HookModuleContext& context)
{
  mouse_control_msg_ = win_throw_on_fail(
      ::RegisterWindowMessageW(WKillMouseControl));
}

cpp::optional<LRESULT> WKillHookModule::processMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
  if (msg == mouse_control_msg_)
  {
    switch ((int)wparam)
    {
    case WKillMouseControlMsg::Start:
      wkillhook->count_ = 0;
      wkillhook->count2_ = 0;
      DebugOutputStream() << POWERWIN_APP_NAME << ": Start mouse control for wkill" << std::endl;
      hook_ = std::make_unique<cpp::scoped_lifetime<WKillHook>>(wkillhook);
      break;

    case WKillMouseControlMsg::Stop:
      DebugOutputStream() << POWERWIN_APP_NAME << ": Stop mouse control for wkill " << wkillhook->count_ << ":" << wkillhook->count2_ << std::endl;
      hook_.reset();
      break;
    }

    return 1;
  }

  return cpp::optional<LRESULT>();
}

HookModuleRegistry::element<WKillHookModule> XWKillHookModule(
  L"wkill", L"kill process with click on window (xkill clone)"
);

} // namespace PowerWin
