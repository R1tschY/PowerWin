#include "utilwindow.h"

#include <memory>

#include "charcodecs.h"
#include "controls/messagesink.h"
#include "debug.h"

namespace Windows {

static LRESULT UtilWndProc(UINT msg, WPARAM wparam, LPARAM lparam) {
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
  return 0;
}

HWND getCallbackWindow() {
  static MessageSink window(UtilWndProc);
  return window.getNativeHandle();
}

} // namespace Windows
