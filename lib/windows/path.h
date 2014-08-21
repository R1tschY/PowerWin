#ifndef PATH_H
#define PATH_H

#include <windows.h>

#include "../c++/stringref.h"

namespace Windows {

inline bool PathExists(cpp::wstring_ref filename) {
  return (GetFileAttributesW(filename.begin()) != INVALID_FILE_ATTRIBUTES);
}

} // namespace Windows

#endif // PATH_H
