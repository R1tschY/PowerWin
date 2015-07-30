/// \file utils.h

#ifndef HOOKLIB_WINDOWS_SHELL_UTILS_H_
#define HOOKLIB_WINDOWS_SHELL_UTILS_H_

#include <shtypes.h>
#include "../com/oleptr.h"

namespace Windows {

#if 1
void getKnownFolderPath(int id, std::wstring& path);
#else
OLEPtr<wchar_t> getKnownFolderPath(REFKNOWNFOLDERID id);
void getKnownFolderPath(REFKNOWNFOLDERID id, std::wstring& path);
#endif

} // namespace Windows

#endif /* HOOKLIB_WINDOWS_SHELL_UTILS_H_ */
