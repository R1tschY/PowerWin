#include "utils.h"

#include <shlobj.h>
#include "../debug.h"

namespace Windows {

#if 1
void getKnownFolderPath(int id, std::wstring& path)
{
  wchar_t result[MAX_PATH];
  win_throw_on_fail(
    SHGetFolderPathW(0, id, nullptr, SHGFP_TYPE_CURRENT, result)
  );
  path.assign(result);
}

#else
OLEPtr<wchar_t> getKnownFolderPath(REFKNOWNFOLDERID id)
{
#if 0
  wchar_t* result;
  win_throw_on_fail( SHGetKnownFolderPath(id, 0, nullptr, &result) );
  return OLEPtr<wchar_t>(result);
#else
  SHGetFolderPathW(0, )
#endif
}

void getKnownFolderPath(REFKNOWNFOLDERID id, std::wstring& path)
{
  auto ptr = getKnownFolderPath(id);
  path.assign(ptr.get());
}
#endif

} // namespace Windows
