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

#ifndef APP_GLOBALEVENTS_H_
#define APP_GLOBALEVENTS_H_

#include <app/signals.h>
#include <windows.h>

#include <cpp-utils/optional.h>


namespace PowerWin {

// TODO: entfernen
class WindowsMessageEvent
{
public:
  WindowsMessageEvent(UINT msg, WPARAM wparam, LPARAM lparam)
  : msg_(msg), wparam_(wparam), lparam_(lparam)
  { }

  UINT getMessage() const { return msg_; }
  WPARAM getWParam() const { return wparam_; }
  LPARAM getLParam() const { return lparam_; }

  void setResult(LRESULT result) { result_ = result; }
  LRESULT getResult() const { return result_.value(); }
  const cpp::optional<LRESULT>& getOptionalResult() const { return result_; }
  bool isResultSet() const { return bool(result_); }

private:
  UINT msg_;
  WPARAM wparam_;
  LPARAM lparam_;
  cpp::optional<LRESULT> result_;
};

class GlobalEvents
{
public:
  using WindowsMessageSignal = Signal<void(WindowsMessageEvent&)>;

  GlobalEvents();
  ~GlobalEvents();

  SignalRegisterInterface<WindowsMessageSignal> getWindowsMessageSignal() { return wm_signal_; }

  cpp::optional<LRESULT> handleWindowsMessage(UINT msg, WPARAM wparam, LPARAM lparam);

private:
  WindowsMessageSignal wm_signal_;
};

} /* namespace PowerWin */

#endif /* APP_GLOBALEVENTS_H_ */
