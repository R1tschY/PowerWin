#ifndef DEBUG_H
#define DEBUG_H

#include <windows.h>
#include <string>

namespace Windows {

enum LogLevel {
  LL_MESSAGE,
  LL_WARNING,
  LL_CRITICAL,
  LL_ERROR,

  LL_MAX
};

#ifdef NDEBUG
# define WIN_MESSAGE(fmt, ...)
#else
# define WIN_MESSAGE(fmt, ...) printMessage(LL_MESSAGE, fmt L"\n", ##__VA_ARGS__)
#endif

#define WIN_WARNING(fmt, ...) printMessage(LL_WARNING, fmt L"\n", ##__VA_ARGS__)
#define WIN_CRITICAL(fmt, ...) printMessage(LL_CRITICAL, fmt L"\n", ##__VA_ARGS__)
#define WIN_ERROR(fmt, ...) printMessage(LL_ERROR, fmt L"\n", ##__VA_ARGS__)

void printMessage(LogLevel level, const wchar_t* format, ...);

std::wstring GetWindowsError(DWORD code);
inline std::wstring
GetLastWindowsError() {
  return GetWindowsError(GetLastError());
}

} // namespace Windows

void print(const wchar_t* format, ...);
void print_window_infos(HWND hwnd);

#endif // DEBUG_H
