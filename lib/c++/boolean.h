#ifndef BOOLEAN_H
#define BOOLEAN_H

#include "stringview.h"

namespace cpp {

cpp::wstring_view to_wstring_view(bool value) {
  return cpp::wstring_view(value ? wstring_literal("yes") : wstring_literal("no"));
}

}

#endif // BOOLEAN_H
