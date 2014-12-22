#include "messagesink.h"

#include <windows.h>

namespace Windows {

static ATOM getMessageSinkClass() {
  static ATOM window_class = Control::registerWindowClass(
                               lit("MessageSink"),
                               0,
                               0,
                               0,
                               0,
                               cpp::string_view(),
                               0);
  return window_class;
}

MessageSink::MessageSink(MessageFunc wndproc) :
  Control(getMessageSinkClass(), WS_POPUP, WS_EX_TOOLWINDOW),
  wndproc_(wndproc)
{
}

void MessageSink::create()
{
  Control::create(HWND_MESSAGE, lit("MessageSink"), 0, 0, 0, 0);
}

LRESULT MessageSink::onMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
  if (wndproc_) {
    return wndproc_(msg, wparam, lparam);
  }

  return Control::onMessage(msg, wparam, lparam);
}


} // namespace Windows
