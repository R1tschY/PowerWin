/*
 * geometrie.h
 *
 *  Created on: 23.02.2015
 *      Author: richard
 */

#ifndef WINDOWS_GEOMETRY_H_
#define WINDOWS_GEOMETRY_H_

#include <windows.h>
#include <gdiplus.h>

namespace Windows {

class Point {
public:
  // constructors

  constexpr Point() noexcept :
  internal_ {0,0}
  {}

  constexpr Point(LONG x, LONG y) noexcept :
  internal_ {x,y}
  {}

  constexpr Point(const Point& other) noexcept :
  internal_ {other.internal_.x,other.internal_.y}
  {}

  constexpr Point(const POINT& other) noexcept :
  internal_ {other.x,other.y}
  {}

  // information

  constexpr LONG getX() const noexcept {
    return internal_.x;
  }

  constexpr LONG getY() const noexcept {
    return internal_.y;
  }

  // native object

  POINT& getNative() {
    return internal_;
  }

  constexpr operator POINT () const noexcept {
    return internal_;
  }

private:
  POINT internal_;
};

class Rectangle {
public:
  // constructors

  constexpr Rectangle() noexcept:
    internal_ {0,0,0,0}
  {}

  constexpr Rectangle(LONG left, LONG top, LONG right, LONG bottom) noexcept :
    internal_ {left,top,right,bottom}
  {}

  constexpr Rectangle(const Rectangle& other) noexcept :
    Rectangle(other.internal_)
  {}

  constexpr Rectangle(const RECT& other) noexcept :
    internal_ {other.left,other.top,other.right,other.bottom}
  {}

  constexpr Rectangle(const Gdiplus::Rect& other) noexcept :
    internal_ {other.X, other.Y, other.X + other.Width, other.Y + other.Height}
  {}

  static Rectangle fromCenter(Point center, int width, int height) noexcept {
    return Rectangle(
        center.getX() - width / 2,
        center.getY() - height / 2,
        center.getX() + width / 2,
        center.getY() + height / 2);
  }

  static Rectangle fromXY(Point xy, int width, int height) noexcept {
    return Rectangle(
        xy.getX(),
        xy.getY(),
        xy.getX() + width,
        xy.getY() + height);
  }

  static Rectangle fromXY(int x, int y, int width, int height) noexcept {
    return Rectangle(
        x,
        y,
        x + width,
        y + height);
  }

  // information

  constexpr Point getCenter() const noexcept {
    return Point((internal_.right+internal_.left)/2, (internal_.bottom+internal_.top)/2);
  }

  constexpr Point getTopLeft() const noexcept {
    return Point(internal_.left, internal_.top);
  }

  constexpr Point getTopRight() const noexcept {
    return Point(internal_.right, internal_.top);
  }

  constexpr Point getBottomRight() const noexcept {
    return Point(internal_.right, internal_.bottom);
  }

  constexpr Point getBottomLeft() const noexcept {
    return Point(internal_.left, internal_.bottom);
  }

  constexpr LONG getX() const noexcept {
    return internal_.left;
  }

  constexpr LONG getY() const noexcept {
    return internal_.top;
  }

  constexpr LONG getWidth() const noexcept {
    return internal_.right-internal_.left;
  }

  constexpr LONG getHeight() const noexcept {
    return internal_.bottom-internal_.top;
  }

  // manipulate

  void inflate(int dx, int dy) noexcept {
    internal_.left -= dx;
    internal_.top -= dy;
    internal_.right += dx;
    internal_.bottom += dy;
  }

  // native object

  RECT& getNative() noexcept {
    return internal_;
  }

  operator RECT () const noexcept {
    return internal_;
  }

  operator Gdiplus::Rect () const noexcept {
    return Gdiplus::Rect(getX(), getY(), getWidth(), getHeight());
  }

private:
  RECT internal_;
};

}
/* namespace Windows */

#endif /* WINDOWS_GEOMETRY_H_ */
