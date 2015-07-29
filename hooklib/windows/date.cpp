#include "date.h"
#include "defines.h"

#include <time.h>

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
  lit("Jan"), lit("Feb"), lit("Mar"),
  lit("Apr"), lit("May"), lit("Jun"),
  lit("Jul"), lit("Aug"), lit("Sep"),
  lit("Oct"), lit("Nov"), lit("Dec")
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
