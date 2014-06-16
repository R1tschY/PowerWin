#ifndef CHARCODECS_H
#define CHARCODECS_H

#include <string>

namespace Windows {

std::string convertToUtf8(const std::wstring& src);

std::wstring convertFromUtf8(const std::string& src);

} // namespace Windows

#endif // CHARCODECS_H
