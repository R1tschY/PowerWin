#include <boost/lexical_cast.hpp>
#include <cstdarg>
#include "Utils.h"

void print(const wchar_t* format, ...) {
  wchar_t buffer[1024];
  va_list vl;

  va_start(vl, format);
  _vsnwprintf_s(buffer, _TRUNCATE, format, vl);
  va_end(vl);

  OutputDebugString(buffer);
}

void print_window_infos(HWND hwnd) {
  OutputDebugStringW(L"---------------------------------------------------------\n");
  wchar_t window_name[255];
  GetWindowText(hwnd, window_name, sizeof(window_name));
  print(L"Fenstertitel:\n\t%s\n", window_name);

  wchar_t class_name[255];
  GetClassNameW(hwnd, class_name, sizeof(class_name));
  print(L"Fensterklassenname:\n\t%s\n", class_name);

  RECT geometry;
  GetWindowRect(hwnd, &geometry);
  OutputDebugString(L"Geometrie:\n");
  print(L"\tx1: %li,\t y1: %li\n", geometry.left, geometry.top);
  print(L"\tx2: %li,\t y2: %li\n", geometry.right, geometry.bottom);

  OutputDebugStringW(L"---------------------------------------------------------\n\n");
}
