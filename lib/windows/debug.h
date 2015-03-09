#ifndef DEBUG_H
#define DEBUG_H

#include <string>

#include <c++/stringview.h>
#include <c++/macros.h>
#include <windows.h>

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

void printError(cpp::wstring_view error_message, DWORD error_code);

//
// Check return value

#undef __PRETTY_FUNCTION__
#define win_throw_on_fail(expr) (checkReturnValue((expr), L" (" TO_WIDESTRING(__FILE__) L":" WSTRINGIFY(__LINE__) L")"))
#define win_return_if_failed(expr,value) \
  WIN_BEGIN_MACRO_BLOCK \
    if (!expr) { \
      return value; \
    } \
  WIN_END_MACRO_BLOCK

template<typename ReturnValue>
inline ReturnValue checkReturnValue(ReturnValue return_value, cpp::wstring_view fail_message) {
  if (!return_value) {
    printError(fail_message, GetLastError());
  }
  return return_value;
}

inline HRESULT checkReturnValue(HRESULT return_value, cpp::wstring_view fail_message) {
  if (return_value != S_OK) {
    printError(fail_message, return_value);
  }
  return return_value;
}



} // namespace Windows



void print(const wchar_t* format, ...);
//void print(const char* format, ...);
void print_window_infos(HWND hwnd);

#endif // DEBUG_H
