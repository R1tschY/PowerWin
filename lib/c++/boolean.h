#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "stringref.h"

namespace cpp {

cpp::wsubstring to_wstring_ref(bool value) {
  return cpp::wsubstring(value ? L"yes" : L"no");
}

}

#endif // BOOLEAN_H
