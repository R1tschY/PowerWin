#ifndef DATE_H
#define DATE_H

#include <string>

#include <c++/stringview.h>
#include <windows.h>

namespace Windows {

std::wstring getDate(cpp::wstring_view format);
void getDate(struct tm* newtime);
std::wstring getDate(cpp::wstring_view format, struct tm* time);
bool getCTime(cpp::wstring_view ctime, struct tm* desttime);

} // namespace Windows

#endif // DATE_H
