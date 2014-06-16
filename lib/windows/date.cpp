#include "date.h"

#include <time.h>

namespace Windows {

std::wstring getDate(const wchar_t* format) {
  __time64_t t;
  _time64(&t);
  return getDate(format, _localtime64(&t));
}

void getDate(tm* newtime) {
  __time64_t t;
  _time64(&t);
  *newtime = *_localtime64(&t);
}

std::wstring getDate(const wchar_t* format, tm* time) {
  wchar_t buffer[64];

  wcsftime(buffer, sizeof(buffer), format, time);

  return std::wstring(buffer);
}

static const wchar_t* mouths[12] = {
  L"Jan", L"Feb", L"Mar", L"Apr", L"May", L"Jun", L"Jul", L"Aug", L"Sep", L"Oct", L"Nov", L"Dec"
};
bool getCTime(const wchar_t* ctime, tm* desttime) {
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
