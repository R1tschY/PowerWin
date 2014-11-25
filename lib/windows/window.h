#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <windows.h>

#include "macros.h"
#include "../c++/zstringref.h"

namespace Windows {

class Window {
  DISALLOW_COPY_AND_ASSIGN(Window);

public:
  typedef HWND Handle;


public:
  Window() : handle_(nullptr) { }
  Window(Window&& other);
  Window(Handle parent,
         cpp::wzstring_ref title,
         DWORD style,
         DWORD ex_style = 0) :
    handle_(createWindow(parent, title, style, ex_style)) { }

  Window& operator=(Window&& other);

  virtual ~Window();

  HWND getNativeHandle() const { return handle_; }
  bool okay() const { return handle_; }
  explicit operator bool () const { return okay(); }

  virtual LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam);

  void init(Handle parent,
            cpp::wzstring_ref title,
            DWORD style,
            DWORD ex_style = 0);

  void destroy();


  const std::wstring& Window::getWindowClassName();


private:
  Handle handle_;

  static ATOM registerWindowClass();
  static ATOM getWindowClass();

  static Handle createWindow(
      wchar_t* window_class,
      Handle parent,
      cpp::wzstring_ref title,
      DWORD style,
      DWORD ex_style);

  static Handle createWindow(
      ATOM window_class,
      Handle parent,
      cpp::wzstring_ref title,
      DWORD style,
      DWORD ex_style) {
    createWindow(MAKEINTATOM(window_class), parent, title, style, ex_style);
  }

  static Handle createWindow(
      Handle parent,
      cpp::wzstring_ref title,
      DWORD style,
      DWORD ex_style) {
    createWindow(getWindowClass(), parent, title, style, ex_style);
  }

  static LRESULT CALLBACK MessageEntry(Handle handle, UINT msg, WPARAM wparam, LPARAM lparam);
};

} // namespace Windows

#endif // WINDOW_H
