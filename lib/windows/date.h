#ifndef DATE_H
#define DATE_H

#include <windows.h>
#include <string>

namespace Windows {

std::wstring getDate(cpp::wstring_view format);
void getDate(struct tm* newtime);
std::wstring getDate(cpp::wstring_view format, struct tm* time);
bool getCTime(cpp::wstring_view ctime, struct tm* desttime);

} // namespace Windows

#endif // DATE_H
