#include "date.h"

#include <time.h>
#include <c++/stringliteral.h>

namespace Windows {

std::wstring getDate(cpp::wstring_view format) {
  __time64_t t;
  _time64(&t);
  return getDate(format, _localtime64(&t));
}

void getDate(tm* newtime) {
  __time64_t t;
  _time64(&t);
  *newtime = *_localtime64(&t);
}

std::wstring getDate(cpp::wstring_view format, tm* time) {
  wchar_t buffer[64];

  wcsftime(buffer, sizeof(buffer), format, time);

  return std::wstring(buffer);
}

static cpp::wstring_view mouths[12] = {
    wstring_literal("Jan"), wstring_literal("Feb"), wstring_literal("Mar"),
    wstring_literal("Apr"), wstring_literal("May"), wstring_literal("Jun"),
    wstring_literal("Jul"), wstring_literal("Aug"), wstring_literal("Sep"),
    wstring_literal("Oct"), wstring_literal("Nov"), wstring_literal("Dec")
};
bool getCTime(cpp::wstring_view ctime, tm* desttime) {
  const wchar_t* p = ctime;
  getDate(desttime);

  // Wochentag:
  p += 4;

  // Monat:
  int i;
  for (i = 0; i < 12; i++) {
    if (wcsncmp(p, mouths[i], 3)==0) break;
  }
  if (i == 12) return false;
  desttime->tm_mon = i;
  p += 4;

  //Tag:
  i = _wtoi(p);
  if (i < 1 || i > 31) return false;
  desttime->tm_mday = i;
  p += 3;

  //hh:mm:ss
  i = _wtoi(p);
  if (i > 23) return false;
  desttime->tm_hour = i;
  p += 3;

  i = _wtoi(p);
  if (i > 59) return false;
  desttime->tm_min = i;
  p += 3;

  i = _wtoi(p);
  if (i > 59) return false;
  desttime->tm_sec = i;
  p += 3;

  //Jahr:
  i = _wtoi(p);
  if (i < 1900) return false;
  desttime->tm_year = i - 1900;

  return (mktime(desttime) != -1);
}

} // namespace Windows
