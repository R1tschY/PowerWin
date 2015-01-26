#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <memory>
#include <type_traits>

#include <c++/stringview.h>
#include <windows.h>

#include "../memory.h"

namespace Windows {

class Color {
public:
  constexpr Color(unsigned char r, unsigned char g, unsigned char b) :
    color_(RGB(r,g,b))
  { }

  constexpr operator COLORREF () const { return color_; }

  constexpr unsigned char getRedChannel() const { return GetRValue(color_); }
  constexpr unsigned char getGreenChannel() const { return GetGValue(color_); }
  constexpr unsigned char getBlueChannel() const { return GetBValue(color_); }

private:
  COLORREF color_;
};

WINDOWS_DEFINE_GENERIC_HANDLE_TYPE(GdiObject, DeleteObject);

class Brush {
public:
  Brush() : handle_(nullptr) { }
  Brush(HBRUSH handle) : handle_(handle) { }

  static Brush fromRGB(unsigned char r, unsigned char g, unsigned char b) {
    return Brush(CreateSolidBrush(RGB(r,g,b)));
  }

  static Brush fromColor(Color color) {
    return Brush(CreateSolidBrush(color));
  }

  static Brush fromSystem(int index) {
    return Brush(GetSysColorBrush(index));
  }

  // TODO: CreateBrushIndirect, CreateDIBPatternBrush, CreateDIBPatternBrushPt, CreateHatchBrush, CreatePatternBrush

  HBRUSH getNativeHandle() {
    return handle_.get();
  }

private:
  GdiObject<HBRUSH> handle_;
};

class Font {
public:
  Font() : handle_(nullptr) { }
  Font(HFONT handle) : handle_(handle) { }
  Font(cpp::wstring_view face,
       int height,
       int width,
       int weight = FW_NORMAL,
       int escapement = 0,
       int orientation = 0,
       bool italic = false,
       bool underline = false,
       bool strikeOut = false,
       DWORD charSet = ANSI_CHARSET,
       DWORD outputPrecision = 0,
       DWORD clipPrecision = 0,
       DWORD quality = DEFAULT_QUALITY,
       DWORD pitchAndFamily = DEFAULT_PITCH) :
    handle_(CreateFont(height, width, escapement, orientation, weight,
              italic, underline, strikeOut, charSet, outputPrecision,
              clipPrecision, quality, pitchAndFamily, face.c_str()))
  { }

  HFONT getNativeHandle() {
    return handle_.get();
  }

private:
  GdiObject<HFONT> handle_;
};

} // namespace Windows

#endif // GRAPHICS_H
