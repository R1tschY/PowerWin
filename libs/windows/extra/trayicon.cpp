#include "trayicon.h"

#include <algorithm>
#include "../base/application.h"

namespace Windows {

int TrayIcon::LastId = 0;

TrayIcon::TrayIcon():
  added_(false)
{}

void
TrayIcon::add(HWND hwnd, HICON icon, cpp::wstring_view tooltip) {
    trayicon_.cbSize = sizeof(NOTIFYICONDATAW);
#if (WINVER < 0x0600)
    trayicon_.uVersion         = NOTIFYICON_VERSION;
    trayicon_.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP;
#else
    trayicon_.uVersion         = NOTIFYICON_VERSION_4;
    trayicon_.uFlags           = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_SHOWTIP;
#endif
    trayicon_.hWnd             = hwnd;
    trayicon_.uID              = ++LastId;
    trayicon_.uCallbackMessage = MessageId;
    trayicon_.hIcon            = icon;

    tooltip.copy_to(trayicon_.szTip);

    Shell_NotifyIconW(NIM_ADD, &trayicon_);
    Shell_NotifyIconW(NIM_SETVERSION, &trayicon_);

    added_ = true;
}

void
TrayIcon::remove() {
  added_ = false;
  Shell_NotifyIconW(NIM_DELETE, &trayicon_);
}

TrayIcon::~TrayIcon(void) {
  if (added_) {
    remove();
  }
}

void
TrayIcon::setToolTip(cpp::wstring_view src) {
#if (WINVER < 0x0600)
  trayicon_.uFlags = NIF_TIP;
#else
  trayicon_.uFlags = NIF_TIP | NIF_SHOWTIP;
#endif
  src.copy_to(trayicon_.szTip);
  Shell_NotifyIconW(NIM_MODIFY, &trayicon_);
}

void
TrayIcon::showBalloon(cpp::wstring_view title, cpp::wstring_view msg, Icon icontype) {
#if (WINVER < 0x0600)
  trayicon_.uFlags = NIF_INFO;
#else
  trayicon_.uFlags = NIF_INFO | NIF_SHOWTIP;
#endif
  trayicon_.dwInfoFlags = static_cast<int>(icontype);
  title.copy_to(trayicon_.szInfoTitle);
  msg.copy_to(trayicon_.szInfo);

  Shell_NotifyIconW(NIM_MODIFY, &trayicon_);
}


} // namespace Windows
