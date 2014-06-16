#include "utilwindow.h"

#include "application.h"
#include "types.h"
#include "charcodecs.h"
#include "debug.h"

namespace Windows {

static ATOM registerUtilWindow() {
  WNDCLASSW wc;

  wc.style         = 0;
  wc.lpfnWndProc   = DefWindowProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = Application::getInstance();
  wc.hIcon         = 0;
  wc.hCursor       = 0;
  wc.hbrBackground = 0;
  wc.lpszMenuName  = 0;
  wc.lpszClassName = L"UtilWindow";

  return RegisterClassW(&wc);
}

HWND createUtilWindow(WNDPROC wndproc) {
  static ATOM wndclass = registerUtilWindow();
  HWND result;

  UNREFERENCED_PARAMETER(wndclass);

  result = CreateWindowExW(WS_EX_TOOLWINDOW, L"UtilWindow",
    L"", WS_POPUP, 0, 0, 0, 0, 0, 0, Application::getInstance(), NULL);
  if (wndproc)
    SetWindowLongPtr(result, GWLP_WNDPROC, (LONG_PTR)wndproc);

  return result;
}

// /////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK UtilWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
  case WM_CALLBACK: {
    Callback* pfunc = (Callback*)lparam;

    if (pfunc != NULL) {
      try {
        (*pfunc)();
      } catch (std::bad_function_call &error) {
        WIN_CRITICAL(L"executeInMainThread scheitert: %s",
                     convertFromUtf8(error.what()).c_str());
      }
      delete pfunc;
    }

    return 0;
  }

  case WM_CREATE:
    // Initialize the window.
  case WM_DESTROY:
    // Clean up window-specific data objects.

  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}

HWND getCallbackWindow() {
  static HWND window = createUtilWindow(UtilWndProc);
  return window;
}

} // namespace Windows
