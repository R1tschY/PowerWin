#include "charcodecs.h"

#include <windows.h>

namespace Windows {

std::string to_string(cpp::wstring_view src) {
  std::string dest;

  if (src.empty())
    return dest;

  int len = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), src.length(), NULL, 0);
  if (len > 1) {
    dest.resize(len);
    WideCharToMultiByte(CP_UTF8, 0, src.c_str(), src.length(), dest.begin(), len, NULL, NULL);
  }

  return dest;
}

std::wstring to_wstring(cpp::wstring_view src) {
  std::wstring dest(src.length(), '\0');

  if (src.empty())
    return dest;

  MultiByteToWideChar(CP_UTF8, 0, src.c_str(), src.length(), dest.begin(), src.length());
  return dest;
}

} // namespace Windows
