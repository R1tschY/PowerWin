#ifndef CHARCODECS_H
#define CHARCODECS_H

#include <c++/stringview.h>

namespace Windows {

std::string convertToUtf8(cpp::wstring_view src);

std::wstring convertFromUtf8(cpp::wstring_view src);

} // namespace Windows

#endif // CHARCODECS_H
