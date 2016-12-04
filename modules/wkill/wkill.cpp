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

#include <app/configuration.h>
#include <app/hotkeymanager.h>
#include <lightports/os/process.h>
#include <lightports/user/cursor.h>
#include <modules/wkill/wkill.h>

using namespace Windows;

namespace PowerWin {

// TODO: add chance to exit

WKill::WKill(ModuleContext& context)
: mouse_hook_(context.getMouseHook()),
  hotkey_(context.getHotkeyManager())
{
  hotkey_.setCallback([=](){ onHotkey(); });
  hotkey_.setKey(context
    .getConfiguration()
    .readValue(L"wkill", L"hotkey", L"Ctrl+Alt+X"));
}

void WKill::onHotkey()
{
  if (state_ != State::Idle)
    return;

  state_ = State::Choose;
  setCursor(loadCursor(StockCursor::Cross));

  hook_connection_ = mouse_hook_.mouseButtonDown().connect(
    [=](Point pt, int button, DWORD time){ return onClick(pt, button, time); }
  );
}

bool WKill::onClick(Point pt, int button, DWORD time)
{
  if (button != 0)
    return false;

  if (state_ != State::Choose)
    return false;

  // TODO: load old cursor?
  setCursor(loadCursor(StockCursor::Normal));

  // kill process of clicked window
  try
  {
    auto hwnd = Window::at(pt);
    auto process = Process::open(
      Process::AccessRights::Terminate,
      hwnd.getProcessId());
    process.terminate(1);
  }
  catch(const Exception& exp)
  {
    DebugOutputStream() << L"WKill: Terminate process failed: " << exp.what() << std::endl;
  }

  state_ = State::Idle;
  hook_connection_.disconnect();

  return true;
}

ModuleRegistry::element<WKill> XWKill(
  L"wkill", L"kill process with click on window (xkill clone)"
);

} // namespace PowerWin
