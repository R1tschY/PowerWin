#ifndef PATH_H
#define PATH_H

#include <windows.h>

#include <c++/stringview.h>

namespace Windows {

class Path {
public:
  explicit Path(cpp::wstring_view path);

  // static interface
  static bool exists(cpp::wstring_view filename);

private:
  const std::wstring path;
};

inline bool Path::exists(cpp::wstring_view filename) {
  return (GetFileAttributesW(filename.c_str()) != INVALID_FILE_ATTRIBUTES);
}

} // namespace Windows

#endif // PATH_H
