#ifndef CONTROL_H
#define CONTROL_H

#include <memory>
#include <string>

#include <windows.h>
#include <c++/stringview.h>
#include "../core/macros.h"
#include "../core/geometry.h"
#include "../core/debug.h"

namespace Windows {

class Control {
  DISALLOW_COPY_AND_ASSIGN(Control);
public:

  // enums

  enum class AnimateMode {
    Blend = AW_BLEND,
    SlideRightToLeft = AW_HOR_NEGATIVE | AW_SLIDE,
    SlideLeftToRight = AW_HOR_POSITIVE | AW_SLIDE,
    SlideTopToBottom = AW_VER_POSITIVE | AW_SLIDE,
    SlideBottomToTop = AW_VER_NEGATIVE | AW_SLIDE,
    RollRightToLeft = AW_HOR_NEGATIVE,
    RollLeftToRight = AW_HOR_POSITIVE,
    RollTopToBottom = AW_VER_NEGATIVE,
    RollBottmToTop = AW_VER_POSITIVE,
    ExpandCollapse = AW_CENTER
  };

  // ctors/dtor
  explicit Control() :
          handle_(nullptr),
          parent_(nullptr),
          class_(MAKEINTATOM(getWindowClass())),
          style_(0),
          exstyle_(0)
  {
  }

  explicit Control(DWORD style, DWORD exstyle = 0) :
          handle_(nullptr),
          parent_(nullptr),
          class_(MAKEINTATOM(getWindowClass())),
          style_(style),
          exstyle_(exstyle)
  {
  }

  explicit Control(const wchar_t* window_class, DWORD style, DWORD exstyle) :
          handle_(nullptr),
          parent_(nullptr),
          class_(window_class),
          style_(style),
          exstyle_(exstyle)
  {
  }

  explicit Control(ATOM window_class, DWORD style, DWORD exstyle) :
          handle_(nullptr),
          parent_(nullptr),
          class_(MAKEINTATOM(window_class)),
          style_(style),
          exstyle_(exstyle)
  {
  }

  Control(Control&& other);
  Control& operator=(Control&& other);

  virtual ~Control();

  // properties

  HWND getNativeHandle() const {
    return handle_.get();
  }

  Rectangle getClientRect() const {
    RECT rc;
    assert(handle_);
    win_print_on_fail(GetClientRect(handle_.get(), &rc));
    return rc;
  }

  static const std::wstring& getWindowClassName();

  bool okay() const {
    return bool(handle_);
  }
  explicit operator bool() const {
    return okay();
  }

  // base functions
  void create(HWND parent, cpp::wstring_view title, const Rectangle& rect) {
    create(parent, title, rect.getX(), rect.getY(), rect.getWidth(), rect.getHeight());
  }
  void create(HWND parent, cpp::wstring_view title, int x, int y, int width, int height);

  void destroy();

  void show(int show_command = SW_SHOW);
  void showAnimated(AnimateMode mode, unsigned time) { AnimateWindow(getNativeHandle(), time, (int)mode); }

  void hide() { show(SW_HIDE); }
  void hideAnimated(AnimateMode mode, unsigned time) { AnimateWindow(getNativeHandle(), time, (int)mode | AW_HIDE); }

  // properties

  void setParent(HWND value) { win_print_on_fail(SetParent(getNativeHandle(), value)); }
  void setParent(Control value) { win_print_on_fail(SetParent(getNativeHandle(), value.getNativeHandle())); }
  HWND getParent() const { return GetParent(getNativeHandle()); }

  void setTopmost();

  void setPosition(const Rectangle& rect);

  // hooks

  virtual LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam);
  virtual void onCreate();
  virtual void onDestroy();

  static ATOM registerClass(
      cpp::wstring_view class_name,
      UINT style,
      HICON icon,
      HCURSOR cursor,
      HBRUSH background,
      cpp::wstring_view menu,
      HICON small_icon);

private:

  struct ControlDeleter {
    typedef HWND pointer;
    void operator()(HWND ptr) {
      DestroyWindow(ptr);
    }
  };
  using Hwnd = std::unique_ptr<HWND, ControlDeleter>;
  using HwndRef = HWND;

  Hwnd handle_;

  HwndRef parent_;
  const wchar_t* class_;
  DWORD style_;
  DWORD exstyle_;

  static ATOM getWindowClass();

  static LRESULT CALLBACK MessageEntry(
      HWND handle,
      UINT msg,
      WPARAM wparam,
      LPARAM lparam);
};

} // namespace Windows

#endif // CONTROL_H
