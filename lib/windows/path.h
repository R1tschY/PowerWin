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

  explicit Path(const wchar_t* begin, const wchar_t* end) :
    path_(begin, end)
  { }

  explicit Path() :
    path_()
  { }

  Path(const Path& path) = default;
  Path& operator=(const Path& path) = default;
  Path(Path&& path) = default;
  Path& operator=(Path&& path) = default;

  // properties
  std::wstring getBasename() const;
  std::wstring getFolder() const;
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
