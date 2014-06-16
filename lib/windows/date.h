#ifndef DATE_H
#define DATE_H

#include <windows.h>
#include <string>

namespace Windows {

std::wstring getDate(const wchar_t* format);
void getDate(struct tm* newtime);
std::wstring getDate(const wchar_t* format, struct tm* time);
bool getCTime(const wchar_t* ctime, struct tm* desttime);

} // namespace Windows

#endif // DATE_H
