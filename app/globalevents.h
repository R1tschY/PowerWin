#ifndef APP_GLOBALEVENTS_H_
#define APP_GLOBALEVENTS_H_

#include <windows.h>

#include <boost/signals2/signal.hpp>
#include <cpp-utils/optional.h>

#include "signal.h"

namespace PowerWin {

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
  typedef boost::signals2::signal<void(WindowsMessageEvent&)> WindowsMessageSignal;

  GlobalEvents();
  ~GlobalEvents();

  SignalRegisterInterface<WindowsMessageSignal> getWindowsMessageSignal() { return wm_signal_; }

  cpp::optional<LRESULT> handleWindowsMessage(UINT msg, WPARAM wparam, LPARAM lparam);

private:
  WindowsMessageSignal wm_signal_;
};

} /* namespace PowerWin */

#endif /* APP_GLOBALEVENTS_H_ */
