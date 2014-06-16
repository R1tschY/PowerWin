#include "charcodecs.h"

#include <windows.h>

namespace Windows {

std::string convertToUtf8(const std::wstring& src) {
  std::string dest;

  if (src.empty())
    return dest;

  int chars = (DWORD)src.length() + 1;
  char *ansi = (char*)malloc(sizeof(char)*(chars+1));

  WideCharToMultiByte(CP_UTF8, 0, src.c_str(), (DWORD)src.length() + 1, ansi, chars, NULL, NULL);

  // sicher ist sicher (vllt Verwächslung mit UTF-8):
  ansi[chars] = '\0';

  dest.assign(ansi);

  free(ansi);

  return dest;
}

std::wstring convertFromUtf8(const std::string& src) {
  std::wstring dest;

  int len = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, NULL, 0);
  if (len > 1) {
    wchar_t *ptr = (wchar_t*)malloc(sizeof(wchar_t) * len);
    MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, ptr, len);
    dest.assign(ptr, len - 1);
    free(ptr);
  }

  return dest;
}

} // namespace Windows
