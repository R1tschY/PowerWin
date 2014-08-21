#include "debug.h"

#include <cerrno>  // für errno
#include <cstdio>  // für printf
#include <sstream>
#include <wchar.h>
#include <string>

#include "../c++/utils.h"

namespace Windows {

std::wstring
GetWindowsError(DWORD code) {
  wchar_t* buffer;
  DWORD bytes;

  bytes = FormatMessageW(
      FORMAT_MESSAGE_ALLOCATE_BUFFER |
      FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      code,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (wchar_t*)&buffer,
      0, NULL);

  if (bytes == 0 || buffer == NULL) {
    std::wstringstream strstream;
    strstream << code;
    return strstream.str();
  }

  std::wstring result(buffer);
  LocalFree(buffer);
  return result;
}

void printMessage(LogLevel level, const wchar_t* format, ...) {
  wchar_t buffer[1024];
  va_list vl;

  va_start(vl, format);
  int result = vsnwprintf(buffer, cpp::length(buffer) - 1, format, vl);
  va_end(vl);

  if (result < 0) {
   // TODO: Error
  }

  static const wchar_t* prefix[LL_MAX] = {
    L"--- MESSAGE: ", // LL_MESSAGE
    L"??? WARNING: ", // LL_WARNING
    L"!!! CRITIAL: ", // LL_CRITICAL
    L"!!! ERROR: ", // LL_ERROR
  };

  OutputDebugStringW(prefix[level]);
  OutputDebugStringW(buffer);

  switch (level) {
  case LL_CRITICAL:
    MessageBoxW(NULL, buffer, L"Kritscher Fehler", MB_OK | MB_ICONWARNING);
    break;
  case LL_ERROR:
    MessageBoxW(NULL, buffer, L"Fataler Fehler", MB_OK | MB_ICONERROR);
    ExitProcess(1);
  default:;
  }
}

} // namespace Windows

void print(const wchar_t* format, ...) {
  wchar_t buffer[1024];
  va_list vl;
  static std::size_t line = 0;
  line++;

  snwprintf(buffer, 5, L"%03d:", line);

  va_start(vl, format);
  vsnwprintf(buffer + 5, 1023 - 5, format, vl);
  va_end(vl);

  buffer[4] = L' ';
  buffer[1023] = L'\0';

  OutputDebugStringW(buffer);
}

/*void print(const char* format, ...) {
  char buffer[1024];
  va_list vl;

  va_start(vl, format);
  vsnprintf(buffer, 1023, format, vl);
  va_end(vl);

  buffer[1023] = '\0';

  OutputDebugStringA(buffer);
}*/

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
