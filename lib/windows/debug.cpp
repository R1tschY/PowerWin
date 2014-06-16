#include "debug.h"

#include <cerrno>  // für errno
#include <cstdio>  // für printf
#include <sstream>

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
  int result = swprintf(buffer, Cpp::length(buffer) - 1, format, vl);
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
