#include "charcodecs.h"

#include <windows.h>

namespace Windows {

std::string toUtf8(cpp::wstring_view src) {
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

std::wstring toWide(cpp::wstring_view src) {
  std::wstring dest(src.length(), '\0');

  if (src.empty())
    return dest;

  MultiByteToWideChar(CP_UTF8, 0, src.c_str(), src.length(), dest.begin(), src.length());
  return dest;
}

//#include <codecvt>
//#include <string>

//// convert UTF-8 string to wstring
//std::wstring utf8_to_wstring (const std::string& str)
//{
//    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
//    return myconv.from_bytes(str);
//}

//// convert wstring to UTF-8 string
//std::string wstring_to_utf8 (const std::wstring& str)
//{
//    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
//    return myconv.to_bytes(str);
//}

} // namespace Windows
