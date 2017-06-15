///
/// Copyright (c) 2017 R1tschY
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#include "wmodify.h"

#include <QString>
#include <QDebug>
#include <cpp-utils/optional.h>

#include <app/mousehook.h>

namespace PowerWin {

namespace {

constexpr int SHIFTED = 0x8000;

cpp::optional<std::tuple<int, int>> parseMouseCombi(const QString& str)
{
  if (str.isEmpty())
    return {};

  auto list = str.toLower().split('+');
  if (list.length() != 2)
    return {};

  int virtualKey = 0xDEADBEEF;
  if (list[0] == QLatin1String("ctrl"))
  {
    virtualKey = VK_CONTROL;
  }
  else if (list[0] == QLatin1String("alt"))
  {
    virtualKey = VK_MENU;
  }
  else if (list[0] == QLatin1String("shift"))
  {
    virtualKey = VK_SHIFT;
  }
  else
  {
    return {};
  }

  int mouseKey = 0xDEADBEEF;
  if (list[1] == QLatin1String("rightbutton"))
  {
    mouseKey = VK_RBUTTON;
  }
  else if (list[1] == QLatin1String("middlebutton"))
  {
    mouseKey = VK_LBUTTON;
  }
  else if (list[1] == QLatin1String("leftbutton"))
  {
    mouseKey = VK_MBUTTON;
  }
  else if (list[1] == QLatin1String("xbutton1"))
  {
    mouseKey = VK_XBUTTON1;
  }
  else if (list[1] == QLatin1String("xbutton2"))
  {
    mouseKey = VK_XBUTTON2;
  }
  else
  {
    return {};
  }

  return std::tuple<int, int>{virtualKey, mouseKey};
}


std::tuple<int, int> parseMouseCombi(const QString& str, const QString& defaultValue)
{
  auto combi = parseMouseCombi(str);
  if (!combi)
  {
    qCritical() << "invalid or unsupported mouse sequence" << str;
    combi = parseMouseCombi(defaultValue);
  }

  return *combi;
}

}  // namespace

using namespace Windows;

WModifyOverlay::WModifyOverlay()
{

}

WModify::WModify(ModuleContext& context)
{
  auto& config = context.getConfiguration();

  std::tie(move_modifer_, move_button_) = parseMouseCombi(
    QString::fromStdWString(
      config.readValue(L"wmodify", L"move", L"alt+leftbutton")),
    QLatin1String("alt+rightbutton")
  );
  std::tie(resize_modifer_, resize_button_) = parseMouseCombi(
    QString::fromStdWString(
      config.readValue(L"wmodify", L"resize", L"alt+middlebutton")),
    QLatin1String("alt+middlebutton")
  );

  qDebug() << "move executed by mouse button:" <<  move_button_
      << "modifer:" << move_modifer_;
  qDebug() << "resize executed by mouse button:" <<  resize_button_
        << "modifer:" << resize_modifer_;

  connect(context.getMouseHook().mouseButtonDown(),
    [=](Point pt, int button, DWORD) { return handleButtonDown(pt, button); });

  connect(context.getMouseHook().mouseMove(),
    [=](Point pt, DWORD time) { return handleMouseMove(pt); });

  connect(context.getMouseHook().mouseButtonUp(),
    [=](Point pt, int button, DWORD) { return handleButtonUp(pt, button); });
}

bool WModify::handleButtonDown(Point pt, int button)
{
  if (state_ != State::Idle)
    return false;

  bool moveState =
      (::GetKeyState(move_modifer_) & SHIFTED) && button == move_button_;
  bool resizeState =
      (::GetKeyState(resize_modifer_) & SHIFTED) && button == resize_button_;
  if (!moveState && !resizeState)
    return false;

  auto hwnd = Window::at(pt);
  if (!hwnd)
    return false;

  hwnd_ = Window(::GetAncestor(hwnd.getHWND(), GA_ROOT));
  if (!hwnd_)
    return false;

  inital_rect_ = hwnd_.getWindowRect();
  initial_pt_ = pt;

  if (resizeState)
  {
    auto cx = initial_pt_.getX() - inital_rect_.getX();
    auto cy = initial_pt_.getY() - inital_rect_.getY();
    float px = float(cx) / float(inital_rect_.getWidth());
    float py = float(cy) / float(inital_rect_.getHeight());

    if (px < py)
    {
      if (px + py < 1.f)
      {
        hresize_ = -1;
        vresize_ = 0;
      }
      else
      {
        hresize_ = 0;
        vresize_ = 1;
      }
    }
    else
    {
      if (px + py < 1.f)
      {
        hresize_ = 0;
        vresize_ = -1;
      }
      else
      {
        hresize_ = 1;
        vresize_ = 0;
      }
    }

    state_ = State::Resizing;
  }
  else
  {
    state_ = State::Moving;
  }

  return true;
}

bool WModify::handleMouseMove(Windows::Point pt)
{
  if (state_ == State::Idle)
    return false;

  if (state_ == State::Resizing)
  {
    auto diffX = pt.getX() - initial_pt_.getX();
    auto diffY = pt.getY() - initial_pt_.getY();

    auto x = inital_rect_.getX();
    auto y = inital_rect_.getY();
    auto cx = inital_rect_.getWidth();
    auto cy = inital_rect_.getHeight();

    if (vresize_ < 0)
    {
      y += diffY;
      cy -= diffY;

      if (cy < 10)
        cy = 10; // TODO: set correct y
    }
    else if (vresize_ > 0)
    {
      cy += diffY;
      if (cy < 10)
        cy = 10;
    }

    if (hresize_ < 0)
    {
      x += diffX;
      cx -= diffX;

      if (cx < 10)
        cx = 10; // TODO: set correct x
    }
    else if (hresize_ > 0)
    {
      cx += diffX;
      if (cx < 10)
        cx = 10;
    }

    ::SetWindowPos(
        hwnd_.getHWND(),
        nullptr,
        x,
        y,
        cx,
        cy,
        SWP_ASYNCWINDOWPOS | SWP_DEFERERASE
    );

    // else no mouse movement
    return false; // TODO: set mouse on yourself
  }
  else if (state_ == State::Moving)
  {
    auto diffX = pt.getX() - initial_pt_.getX();
    auto diffY = pt.getY() - initial_pt_.getY();

    ::SetWindowPos(
        hwnd_.getHWND(),
        nullptr,
        inital_rect_.getX() + diffX,
        inital_rect_.getY() + diffY,
        0,
        0,
        SWP_ASYNCWINDOWPOS | SWP_NOSIZE
    );

    // else no mouse movement
    return false; // TODO: set mouse on yourself
  }

  return false;
}

bool WModify::handleButtonUp(Windows::Point pt, int button)
{
  if (state_ == State::Idle)
    return false;

  // handle last mouse move
  handleMouseMove(pt);

  state_ = State::Idle;
  return false;
}

ModuleRegistry::element<WModify> XWModify(
  L"wmodify", L"move and resize windows by shortcut and mouse move"
);

} /* namespace PowerWin */
