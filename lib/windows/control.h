#ifndef CONTROL_H
#define CONTROL_H

#include <memory>
#include <string>
#include <windows.h>

#include "macros.h"
#include <c++/stringview.h>

namespace Windows {

class Control {
  DISALLOW_COPY_AND_ASSIGN(Control);

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

  // ctors/dtor
  explicit Control::Control() :
    handle_(nullptr), parent_(nullptr), class_(MAKEINTATOM(getWindowClass())),
    style_(0), exstyle_(0)
  { }

  explicit Control::Control(DWORD style, DWORD exstyle = 0) :
    handle_(nullptr), parent_(nullptr), class_(MAKEINTATOM(getWindowClass())),
    style_(style), exstyle_(exstyle)
  { }

  explicit Control::Control(const wchar_t* window_class, DWORD style = 0, DWORD exstyle = 0) :
    handle_(nullptr), parent_(nullptr), class_(window_class),
    style_(style), exstyle_(exstyle)
  { }

  explicit Control::Control(ATOM window_class, DWORD style = 0, DWORD exstyle = 0) :
    handle_(nullptr), parent_(nullptr), class_(MAKEINTATOM(window_class)),
    style_(style), exstyle_(exstyle)
  { }

  Control(Control&& other);

  Control& operator=(Control&& other);

  virtual ~Control();

  // properties

  HWND getNativeHandle() const { return handle_; }

  const std::wstring& getWindowClassName();


  bool okay() const { return handle_; }
  explicit operator bool () const { return okay(); }

  // base functions
  void create(HWND parent, cpp::wstring_view title, int x, int y, int width, int height);
  void show(int show_command);
  void destroy();

  // virtual functions

  virtual LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam);
  virtual void onCreate();
  virtual void onDestroy();

  static ATOM registerClass(
      cpp::string_view class_name,
      UINT style,
      HICON icon,
      HCURSOR cursor,
      HBRUSH background,
      cpp::string_view menu,
      HICON small_icon);

private:
  Hwnd handle_;

  HwndRef parent_;
  const wchar_t* class_;
  DWORD style_;
  DWORD exstyle_;

  static ATOM getWindowClass();

  static LRESULT CALLBACK MessageEntry(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);
};

} // namespace Windows

#endif // CONTROL_H
