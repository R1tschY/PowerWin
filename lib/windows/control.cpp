#include "control.h"

#include "application.h"
#include "debug.h"


namespace Windows {

//
// Window class

ATOM Control::registerClass(
    cpp::string_view class_name,
    UINT style,
    HICON icon,
    HCURSOR cursor,
    HBRUSH background,
    cpp::string_view menu,
    HICON small_icon)
{
  WNDCLASSEX wcex = { };

  wcex.cbSize        = sizeof(WNDCLASSEX);
  wcex.style		     = style;
  wcex.lpfnWndProc	 = &Control::MessageEntry;
  wcex.cbClsExtra	   = 0;
  wcex.cbWndExtra	   = 0;
  wcex.hInstance	   = Application::getInstance();
  wcex.hIcon		     = icon;
  wcex.hIconSm		   = small_icon;
  wcex.hCursor		   = cursor;
  wcex.hbrBackground = background;
  wcex.lpszClassName = class_name;
  wcex.lpszMenuName	 = menu.c_str();

  return win_check(RegisterClassEx(&wcex));
}

ATOM Control::getWindowClass() {
  static ATOM window_class = registerWindowClass(
                               getWindowClassName().c_str(),
                               0,
                               0,
                               LoadCursor(nullptr, IDC_ARROW),
                               (HBRUSH)(COLOR_WINDOW+1),
                               cpp::string_view(),
                               0);
  return window_class;
}

const std::wstring& Control::getWindowClassName() {
  static const std::wstring window_class_name = Application::getName() + L" Window";
  return window_class_name;
}

//
// ctors/dtor

Control::Control(Control&& other) :
  handle_(std::move(other.handle_))
{
  if (handle_) {
    SetWindowLongPtr(handle_, GWLP_USERDATA, this);
  }
}

Control::~Control()
{ }

Control& Control::operator=(Control&& other)
{
  handle_ = std::move(other.handle_);
  SetWindowLongPtr(handle_, GWLP_USERDATA, this);
}

// base functions

void Control::create(
    HWND parent,
    cpp::wstring_view title,
    int x, int y, int width, int height)
{
  handle_ =
      CreateWindowExW(
        // Optional window styles
        exstyle_,

        // Window class
        class_,

        // Window text
        title,

        // Window style
        style_,

        // Size and position
        x, y, width, height,

        // Parent window
        parent,

        // Menu
        nullptr,

        // Instance handle
        Application::getInstance(),

        // Additional application data
        this);

  // TODO: error checking
}

void Control::show(int show_command)
{
  if (!handle_) return;

  ShowWindow(handle_, show_command);
}

void Control::destroy() {
  handle_ = nullptr;
}

//
// message handling

LRESULT Control::onMessage(UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
  case WM_CREATE:
    onCreate();
    return 0;

  case WM_DESTROY:
    onDestroy();
    return 0;

  default:
    return DefWindowProc(handle_.get(), uMsg, wParam, lParam);
  }
}

void Control::onCreate()
{ }

void Control::onDestroy()
{ }

LRESULT CALLBACK Control::MessageEntry(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) {
  auto window = static_cast<Control*>(GetWindowLongPtr(handle, GWLP_USERDATA));
  if (!window) {
    if (msg == WM_NCCREATE) {
      auto create_struct = reinterpret_cast<CREATESTRUCT*>(lParam);
      window = static_cast<Control*>(create_struct->lpCreateParams);
      SetWindowLongPtr(handle, GWLP_USERDATA, window);
    }

    if (!window)
    {
      // TODO: error message
      return DefWindowProc(handle, msg, wparam, lparam);
    }
  }

  return window->onMessage(msg, wparam, lparam);
}

} // namespace Windows
