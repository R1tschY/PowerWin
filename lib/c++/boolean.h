#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "stringref.h"

namespace cpp {

cpp::wstring_ref to_wstring_ref(bool value) {
  return cpp::wstring_ref(value ? L"yes" : L"no");
}

}

#endif // BOOLEAN_H
