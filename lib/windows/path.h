#ifndef PATH_H
#define PATH_H

#include <string>

#include <c++/stringview.h>
#include "macros.h"
#include <windows.h>

namespace Windows {

class Path {
public:
  // ctors
  explicit Path(cpp::wstring_view path) :
    path_(path.to_string())
  { }

  Path(const Path& path) = default;
  Path& operator=(const Path& path) = default;
  Path(Path&& path) = default;
  Path& operator=(Path&& path) = default;

  // properties
  cpp::string_view getBasename();
  cpp::string_view getFolder();
  std::wstring toString() const { return path_; }

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
