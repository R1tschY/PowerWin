#include "path.h"

#include <cassert>

namespace Windows {

std::wstring Windows::Path::getBasename() const
{
  auto last_sep = path_.find_last_of(L"/\\");
  if (last_sep == std::wstring::npos) {
    return path_;
  }

  return path_.substr(last_sep + 1);
}

std::wstring Windows::Path::getFolder() const
{
  auto last_sep = path_.find_last_of(L"/\\");
  if (last_sep == std::wstring::npos) {
    return path_;
  }

  return path_.substr(0, last_sep);
}

} // namespace Windows
