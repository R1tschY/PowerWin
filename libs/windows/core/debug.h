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
# define WIN_MESSAGE(fmt, ...) printMessage(::Windows::LL_MESSAGE, fmt L"\n", ##__VA_ARGS__)
#endif

#define WIN_WARNING(fmt, ...) printMessage(::Windows::LL_WARNING, fmt L"\n", ##__VA_ARGS__)
#define WIN_CRITICAL(fmt, ...) printMessage(::Windows::LL_CRITICAL, fmt L"\n", ##__VA_ARGS__)
#define WIN_ERROR(fmt, ...) printMessage(::Windows::LL_ERROR, fmt L"\n", ##__VA_ARGS__)

void printMessage(LogLevel level, const wchar_t* format, ...);

//
// Windows API error

std::wstring GetWindowsError(DWORD code);

inline std::wstring
GetLastWindowsError() {
  return GetWindowsError(GetLastError());
}

void printError(cpp::wstring_view error_message, DWORD error_code);

void printWindowsFail(const char* func, const char* error_message, DWORD error_code);
void throwWindowsFail(const char* func, const char* error_message, DWORD error_code);

//
// Check return value
#define win_throw_fail() win_throw_on_fail(false)
#define win_print_fail() win_print_on_fail(false)
#define win_throw_on_fail(expr) (::Windows::checkReturnValue((expr), __PRETTY_FUNCTION__, "[" __FILE__ ":" STRINGIFY(__LINE__) "]", ::Windows::throwWindowsFail))
#define win_print_on_fail(expr) (::Windows::checkReturnValue((expr), __PRETTY_FUNCTION__, "[" __FILE__ ":" STRINGIFY(__LINE__) "]", ::Windows::printWindowsFail))
#define win_return_if_failed(expr,value) \
  WIN_BEGIN_MACRO_BLOCK \
    if (!expr) { \
      return value; \
    } \
  WIN_END_MACRO_BLOCK

template<typename ReturnValue, typename FailureFunc>
inline ReturnValue checkReturnValue(ReturnValue return_value, const char* func, const char* fail_message, FailureFunc failfunc) {
  if (!return_value) {
    failfunc(func, fail_message, GetLastError());
  }
  return return_value;
}

template<typename FailureFunc>
inline HRESULT checkReturnValue(HRESULT return_value, const char* func, const char* fail_message, FailureFunc failfunc) {
  if (return_value != S_OK) {
    failfunc(func, fail_message, return_value);
  }
  return return_value;
}

} // namespace Windows



void print(const wchar_t* format, ...);
//void print(const char* format, ...);
void print_window_infos(HWND hwnd);

#endif // DEBUG_H
