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
#include <app/log.h>
#include <lightports/os/process.h>
#include <lightports/user/cursor.h>
#include <modules/wkill/wkill.h>

using namespace Windows;

namespace PowerWin {

// TODO: add chance to exit

WKill::WKill(ModuleContext& context)
: mouse_hook_(context.getMouseHook()),
  hotkey_(context.getHotkeyManager()),
  state_(State::Idle)
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

  log(Info) << L"wkill: choose window!" << std::endl;

  state_ = State::Choose;

  CursorView cross = loadCursor(StockCursor::Cross);

//  for (DWORD cursorId : {OCR_APPSTARTING, OCR_NORMAL, OCR_CROSS, OCR_HAND,
//    OCR_HELP, OCR_IBEAM, OCR_NO, OCR_SIZEALL, OCR_SIZENESW, OCR_SIZENS,
//    OCR_SIZENWSE, OCR_SIZEWE, OCR_UP, OCR_WAIT})
//  {
//    CursorHandle cross_copy = cross.copy();
//    win_print_on_fail(SetSystemCursor(cross_copy.release(), cursorId));
//  }

  //log(Info) << L"LoadCursor " << cursor << std::endl;
  //log(Info) << L"SetCursor " << ::SetCursor(cursor) << std::endl;

  setCursor(cross);

  hook_connection_ = mouse_hook_.mouseButtonUp().connect(
    [=](Point pt, int button, DWORD time){ return onClick(pt, button, time); }
  );
  hook_connection2_ = mouse_hook_.mouseMove().connect(
    [=](Point pt, DWORD time){ return onMove(pt, time); }
  );
}

bool WKill::onMove(Point pt, DWORD time)
{
  // set cursor
  setCursor(loadCursor(StockCursor::Cross));

  // do not pass move events to the apps
  return true;
}

bool WKill::onClick(Point pt, int button, DWORD time)
{
  if (state_ != State::Choose)
    return false;

  state_ = State::Idle;
  hook_connection_.disconnect();
  hook_connection2_.disconnect();

  // TODO: load old cursor?
//  SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);

  if (button == 0)
  {
    // kill process of clicked window
    try
    {
      auto hwnd = Window::at(pt);
      auto pid = hwnd.getProcessId();
      auto process = Process::open(Process::AccessRights::Terminate, pid);

      log(Info) << L"wkill: terminate HWND: " << hwnd.getHWND() << L" PID: " << std::dec << pid << std::endl;
      process.terminate(1);
    }
    catch(const Exception& exp)
    {
      log(Error) << L"wkill: Terminate process failed: " << exp.what() << std::endl;
    }
  }
  else
  {
    log(Info) << L"wkill: canceled" << std::endl;
  }

  return true;
}

ModuleRegistry::element<WKill> XWKill(
  L"wkill", L"kill process with click on window (xkill clone)"
);

} // namespace PowerWin
