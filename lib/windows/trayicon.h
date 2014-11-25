#ifndef TRAYICON_H
#define TRAYICON_H

#include <windows.h>
#include <shellapi.h>

#include "../c++/stringref.h"

namespace Windows {

class TrayIcon {
public:
  enum class Icon {
    None    = NIIF_NONE,
    Info    = NIIF_INFO,
    Warning = NIIF_WARNING,
    Error   = NIIF_ERROR
  };

  static const unsigned int MessageId = WM_USER + 21;

  TrayIcon();
  ~TrayIcon();

  void add(HWND hwnd, HICON icon);
  void add(HWND hwnd, HICON icon, cpp::wstring_ref tooltip);
  void remove();

  void setToolTip(cpp::wstring_ref src);
  void showBalloon(cpp::wstring_ref title,
                   cpp::wstring_ref msg,
                   Icon icontype);

private:
  NOTIFYICONDATAW trayicon_;
  bool added_;

  static int LastId;
};

} // namespace Windows

#endif // TRAYICON_H
