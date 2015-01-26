#include "charcodecs.h"

#include <stdexcept>

#include <windows.h>

namespace Windows {

std::string to_string(cpp::wstring_view str) {
  if (str.empty())
     return std::string();

  size_t charsNeeded = ::WideCharToMultiByte(CP_UTF8, 0,
     str.data(), (int)str.size(), nullptr, 0, nullptr, nullptr);
  if (charsNeeded == 0)
     throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

  char buffer[charsNeeded];
  int charsConverted = ::WideCharToMultiByte(CP_UTF8, 0,
     str.data(), (int)str.size(), buffer, charsNeeded, nullptr, nullptr);
  // TODO: if (charsConverted == 0) throw new Windows::Exception(); aka THROW_WINDOWS_EXCEPTION()
  // TODO aka WINDOWS_CHECKED(::MultiByteToWideChar)
  if (charsConverted == 0)
     throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

  return std::string(buffer, charsConverted);
}

std::wstring to_wstring(cpp::string_view str) {
  if (str.empty())
     return std::wstring();

  size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
     str.data(), (int)str.size(), nullptr, 0);
  if (charsNeeded == 0)
     throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

  wchar_t buffer[charsNeeded];
  int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
     str.data(), (int)str.size(), buffer, charsNeeded);
  // TODO: if (charsConverted == 0) throw new Windows::Exception(); aka THROW_WINDOWS_EXCEPTION()
  // TODO aka WINDOWS_CHECKED(::MultiByteToWideChar)
  if (charsConverted == 0)
     throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

  return std::wstring(buffer, charsConverted);
}

//#include <codecvt>
//#include <string>

//// convert UTF-8 string to wstring
//std::wstring to_wstring (const std::string& str)
//{
//    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
//    return myconv.from_bytes(str);
//}

//// convert wstring to UTF-8 string
//std::string to_string (const std::wstring& str)
//{
//    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
//    return myconv.to_bytes(str);
//}

} // namespace Windows
