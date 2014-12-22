#ifndef PATH_H
#define PATH_H

#include <windows.h>
#include <string>

#include <c++/stringview.h>
#include "macros.h"

namespace Windows {

class Path {
public:
  // ctors
  explicit Path(cpp::wstring_view path) :
    path_(path)
  { }

  explicit Path(const Path& path) = default;
  Path& operator=(const Path& path) = default;
  explicit Path(const Path&& path) = default;
  Path& operator=(Path&& path) = default;

  // properties
  cpp::string_view getBasename();
  cpp::string_view getFolder();

  // filesystem
  bool exists() const { return exists(path_); }

  // static interface
  static bool exists(cpp::wstring_view filename);

private:
  const std::wstring path_;
};

inline bool Path::exists(cpp::wstring_view filename) {
  return (GetFileAttributesW(filename.c_str()) != INVALID_FILE_ATTRIBUTES);
}

} // namespace Windows

#endif // PATH_H
