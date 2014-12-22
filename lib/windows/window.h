#ifndef WINDOW_H
#define WINDOW_H

#include <memory>
#include <string>
#include <windows.h>

#include "macros.h"
#include <c++/stringview.h>

namespace Windows {

class Window {
  DISALLOW_COPY_AND_ASSIGN(Window);

  class HandleDeleter
  {
  public:
    typedef HWND pointer;

    void operator()(HWND ptr)
    {
      DestroyWindow(ptr);
    }
  };

  template<typename T>
  using Hwnd = std::unique_ptr<HWND, HwndDeleter>;

  using HwndRef = HWND;

public:
  Window() : handle_(nullptr) { }
  Window(Window&& other);

  Window& operator=(Window&& other);

  virtual ~Window();

  HWND getNativeHandle() const { return handle_; }


  bool okay() const { return handle_; }
  explicit operator bool () const { return okay(); }

  virtual LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam);

  void destroy();

  void create(HWND parent, cpp::wstring_view title, DWORD style, DWORD ex_style = 0);
  void createMessageOnly(cpp::wstring_view title, DWORD style, DWORD ex_style = 0) {
    create(HWND_MESSAGE, title, style, ex_style);
  }

  const std::wstring& getWindowClassName();


  // static interface

  static void processMessages();



private:
  Hwnd handle_;
  HwndRef parent_;
  std::wstring title;
  DWORD style;
  DWORD ex_style;

  static ATOM registerWindowClass();
  static ATOM getWindowClass();

  static Hwnd createWindow(
      wchar_t* window_class,
      HWND parent,
      cpp::wstring_view title,
      DWORD style,
      DWORD ex_style);

//  static Hwnd createWindow(
//      ATOM window_class,
//      HWND parent,
//      cpp::wstring_view title,
//      DWORD style,
//      DWORD ex_style) {
//    return createWindow(MAKEINTATOM(window_class), parent, title, style, ex_style);
//  }

//  static Hwnd createWindow(
//      HWND parent,
//      cpp::wstring_view title,
//      DWORD style,
//      DWORD ex_style) {
//    return createWindow(getWindowClass(), parent, title, style, ex_style);
//  }

  static LRESULT CALLBACK MessageEntry(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
};

} // namespace Windows

#endif // WINDOW_H
