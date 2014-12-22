#include "window.h"

#include "application.h"
#include "debug.h"


namespace Windows {

//
// Window class

ATOM Window::registerWindowClass() {
  WNDCLASSEX wcex = { };

  wcex.cbSize        = sizeof(WNDCLASSEX);
  wcex.style		     = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc	 = &Window::MessageEntry;
  wcex.cbClsExtra	   = 0;
  wcex.cbWndExtra	   = 0;
  wcex.hInstance	   = Application::getInstance();
  wcex.hIcon		     = 0;
  wcex.hIconSm		   = 0;
  wcex.hCursor		   = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName	 = 0;
  wcex.lpszClassName = getWindowClassName().c_str();

  return win_check(RegisterClassEx(&wcex));
}

ATOM Window::getWindowClass() {
  static ATOM window_class = registerWindowClass();
  return window_class;
}

const std::wstring& Window::getWindowClassName() {
  static const std::wstring window_class_name = Application::getName() + L" Window";
  return window_class_name;
}

//
// Window

Window::Window(Window&& other) :
  handle_(std::move(other.handle_))
{
  SetWindowLongPtr(handle_, GWLP_USERDATA, this);
}

Window::~Window()
{ }

Window& Window::operator=(Window&& other)
{
  handle_ = std::move(other.handle_);
  SetWindowLongPtr(handle_, GWLP_USERDATA, this);
}

Window::Handle Window::createWindow(
    wchar_t* window_class,
    HWND parent,
    cpp::wstring_view title,
    DWORD style,
    DWORD ex_style)
{
  Handle handle =
      CreateWindowExW(
        // Optional window styles
        ex_style,

        // Window class
        window_class,

        // Window text
        title,

        // Window style
        style,

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        // Parent window
        parent,

        // Menu
        nullptr,

        // Instance handle
        Application::getInstance(),

        // Additional application data
        this
        );

  // TODO: error checking
  return handle;
}

void Window::create(HWND parent, cpp::wstring_view title, DWORD style, DWORD ex_style)
{
  handle_ = createWindow(parent, title, style, ex_style);
}

void Window::destroy() {
  handle_ = nullptr;
}

//
// message handling

LRESULT Window::onMessage(UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;

  default:
    return DefWindowProc(handle_.get(), uMsg, wParam, lParam);
  }
}

LRESULT CALLBACK Window::MessageEntry(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) {
  auto window = static_cast<Window*>(GetWindowLongPtr(handle, GWLP_USERDATA));
  if (!window) {
    if (msg == WM_NCCREATE) {
      auto create_struct = reinterpret_cast<CREATESTRUCT*>(lParam);
      window = static_cast<Window*>(create_struct->lpCreateParams);
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

void Window::processMessages()
{
  // main message loop
  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

} // namespace Windows
