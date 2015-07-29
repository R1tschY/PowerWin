#include "charcodecs.h"

#include <stdexcept>

#include <windows.h>

namespace Windows {

std::wstring format_string(const wchar_t* fmt, ...) {

}

std::string to_string(cpp::wstring_view str) {
  std::string result;

  if (str.empty())
     return result;

  size_t charsNeeded = ::WideCharToMultiByte(CP_UTF8, 0,
     str.data(), (int)str.size(), nullptr, 0, nullptr, nullptr);
  if (charsNeeded == 0)
     throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

  result.append(charsNeeded, '\0');
  int charsConverted = ::WideCharToMultiByte(CP_UTF8, 0,
     str.data(), (int)str.size(), &result[0], result.size(), nullptr, nullptr);
  // TODO: if (charsConverted == 0) throw new Windows::Exception(); aka THROW_WINDOWS_EXCEPTION()
  // TODO aka WINDOWS_CHECKED(::MultiByteToWideChar)
  if (charsConverted == 0)
     throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

  return result;
}

std::wstring to_wstring(cpp::string_view str) {
  std::wstring result;

  if (str.empty())
     return result;

  size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
     str.data(), (int)str.size(), nullptr, 0);
  if (charsNeeded == 0)
     throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

  result.append(charsNeeded, L'\0');
  int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
     str.data(), (int)str.size(), &result[0], result.size());
  // TODO: if (charsConverted == 0) throw new Windows::Exception(); aka THROW_WINDOWS_EXCEPTION()
  // TODO aka WINDOWS_CHECKED(::MultiByteToWideChar)
  if (charsConverted == 0)
     throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

  return result;
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
