#include "timeout.h"

#include <memory>
#include <exception>

#include "../core/debug.h"
#include "../base/charcodecs.h"
#include "../controls/utilwindow.h"

namespace Windows {

Timeout::Timeout(const Callback& callback, int milliseconds):
  callback_(callback), intervall_(milliseconds), enabled_(false)
{ }

Timeout::~Timeout()
{
  stop();
}

void Timeout::start()
{
  stop();

  if (intervall_ <= 0)
    return;

  win_throw_on_fail(
    SetTimer(getCallbackWindow(), (UINT_PTR)this, intervall_, ccallback)
  );
  enabled_ = true;
}

void Timeout::stop()
{
  if (enabled_)
  {
    KillTimer(getCallbackWindow(), (UINT_PTR)this);
    enabled_ = false;
  }
}

void Timeout::setInterval(int milliseconds)
{
  intervall_ = milliseconds;
  if (enabled_)
    start();
}

void Timeout::setCallback(const Callback& callback)
{
  callback_ = callback;
  if (enabled_)
    start();
}

void Timeout::execute(const Callback& callback, int milliseconds) {
  UINT_PTR cb = reinterpret_cast<UINT_PTR>(new Callback(callback));
  win_throw_on_fail(
    SetTimer(getCallbackWindow(), cb, milliseconds, cexecallback)
  );
}

void CALLBACK Timeout::ccallback(HWND, UINT msg, UINT_PTR data, DWORD)
{
  if (msg != WM_TIMER)
    return;

  auto self = reinterpret_cast<Timeout*>(data);
  assert(self != nullptr);

  try
  {
    if (self->enabled_)
      self->callback_();
  }
  catch (const std::bad_function_call& error)
  {
    print(L"timeout callback call failed: %s\n",  error.what());
  }
}

void CALLBACK Timeout::cexecallback(HWND, UINT msg, UINT_PTR callback, DWORD)
{
  if (msg != WM_TIMER) return;

  auto cb = std::unique_ptr<Callback>(reinterpret_cast<Callback*>(callback));
  assert(cb != nullptr);

  try
  {
    (*cb)();
  }
  catch (const std::bad_function_call& error)
  {
    print(L"timeout callback call failed: %s\n", error.what());
  }

  KillTimer(getCallbackWindow(), callback);
}

} // namespace Windows
