#ifndef TRAYICON_H
#define TRAYICON_H

#include <c++/stringview.h>
#include <windows.h>
#include <shellapi.h>

#include "../core/macros.h"

namespace Windows {

class TrayIcon {
  DISALLOW_COPY_AND_ASSIGN(TrayIcon);

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

  void add(HWND hwnd, HICON icon, cpp::wstring_view tooltip);
  void remove();

  void setToolTip(cpp::wstring_view src);
  void showBalloon(cpp::wstring_view title,
                   cpp::wstring_view msg,
                   Icon icontype);

private:
  NOTIFYICONDATAW trayicon_;
  bool added_;

  static int LastId;
};

} // namespace Windows

#endif // TRAYICON_H
