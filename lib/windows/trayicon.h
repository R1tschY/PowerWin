#ifndef TRAYICON_H
#define TRAYICON_H

#include <windows.h>
#include <shellapi.h>

#include "../c++/stringref.h"

namespace Windows {

class TrayIcon {
public:
  typedef enum class {
    None    = NIIF_NONE,
    Info    = NIIF_INFO,
    Warning = NIIF_WARNING,
    Error   = NIIF_ERROR
  } IconType;

  static const unsigned int MessageId = WM_USER + 21;

  TrayIcon();
  ~TrayIcon();

  void add(HWND hwnd, HICON icon);
  void remove();

  void setToolTip(Cpp::WStringRef src);
  void showBalloon(Cpp::WStringRef title,
                   Cpp::WStringRef msg,
                   IconType icontype);

private:
  NOTIFYICONDATAW trayicon_;
  bool added_;

  static int LastId;
};

} // namespace Windows

#endif // TRAYICON_H
