#ifndef WINDOWS_H
#define WINDOWS_H

#include <windows.h>

namespace Windows {

enum WindowsVersion {
  WIN_2000 = 0x0500,
  WIN_XP = 0x0501,
  WIN_VISTA = 0x0600,
  WIN_7 = 0x0601,

  WIN_NEWER = 0xFFFF
};

} // namespace Windows

#endif // WINDOWS_H
