#ifndef CHARCODECS_H
#define CHARCODECS_H

#include <string>

#include <c++/stringview.h>

namespace Windows {

inline std::string to_string(const std::string& src) { return src; }
std::string to_string(cpp::wstring_view src);

inline std::wstring to_wstring(const std::wstring& src) { return src; };
std::wstring to_wstring(cpp::string_view src);

} // namespace Windows

#endif // CHARCODECS_H
