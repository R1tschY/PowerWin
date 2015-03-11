#include "utilwindow.h"

#include <memory>

#include "charcodecs.h"
#include "controls/messagesink.h"
#include "debug.h"

namespace Windows {

static LRESULT UtilWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
  case WM_CALLBACK:
    if (lparam) {
      auto pfunc = std::unique_ptr<Callback>((Callback*)lparam);
      try {
        (*pfunc)();
      } catch (std::bad_function_call &error) {
        WIN_CRITICAL(L"callback failed: %s",
                     to_wstring(error.what()).c_str());
      }
    }
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

static MessageSink createCallbackWindow() {
  MessageSink message_sink(UtilWndProc);
  message_sink.create();
  return message_sink;
}

HWND getCallbackWindow() {
  static MessageSink window = createCallbackWindow();
  assert(window.getNativeHandle());
  return window.getNativeHandle();
}

} // namespace Windows
