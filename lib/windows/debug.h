#ifndef DEBUG_H
#define DEBUG_H

#include <windows.h>
#include <string>

#include "../c++/zstringref.h"

namespace Windows {

enum LogLevel {
  LL_MESSAGE,
  LL_WARNING,
  LL_CRITICAL,
  LL_ERROR,

  LL_MAX
};

//
// Print debug message

#ifdef NDEBUG
# define WIN_MESSAGE(fmt, ...)
#else
# define WIN_MESSAGE(fmt, ...) printMessage(LL_MESSAGE, fmt L"\n", ##__VA_ARGS__)
#endif

#define WIN_WARNING(fmt, ...) printMessage(LL_WARNING, fmt L"\n", ##__VA_ARGS__)
#define WIN_CRITICAL(fmt, ...) printMessage(LL_CRITICAL, fmt L"\n", ##__VA_ARGS__)
#define WIN_ERROR(fmt, ...) printMessage(LL_ERROR, fmt L"\n", ##__VA_ARGS__)

void printMessage(LogLevel level, const wchar_t* format, ...);

//
// Windows API error

std::wstring GetWindowsError(DWORD code);
inline std::wstring
GetLastWindowsError() {
  return GetWindowsError(GetLastError());
}

void printLastError(cpp::wzstring_ref error_message);

//
// Check return value

#define win_check(expr) (checkReturnValue((expr), __PRETTY_FUNCTION__ " (" __FILE__ ":" __LINE__ ")"))
#define win_return_if_failed(expr,value) \
  WIN_BEGIN_MACRO_BLOCK \
    if (!expr) { \
      return value; \
    } \
  WIN_END_MACRO_BLOCK

template<typename ReturnValue>
inline checkReturnValue(ReturnValue return_value, cpp::wzstring_ref fail_message) {
  if (!return_value) {
    printLastError(fail_message);
  }
  return return_value;
}



} // namespace Windows



void print(const wchar_t* format, ...);
//void print(const char* format, ...);
void print_window_infos(HWND hwnd);

#endif // DEBUG_H
