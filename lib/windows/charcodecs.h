#ifndef CHARCODECS_H
#define CHARCODECS_H

#include <c++/stringview.h>

namespace Windows {

namespace CharCodecs {

std::string toUtf8(cpp::string_view src) { return src; }
std::string toUtf8(cpp::wstring_view src);

std::wstring toWide(cpp::string_view src);
std::wstring toWide(cpp::wstring_view src) { return src; }

} // namespace CharCodecs

} // namespace Windows

#endif // CHARCODECS_H
