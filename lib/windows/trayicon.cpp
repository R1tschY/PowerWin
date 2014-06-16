#include "trayicon.h"

#include "application.h"

namespace Windows {

int TrayIcon::LastId = 0;

TrayIcon::TrayIcon():
  added_(false)
{}

void
TrayIcon::add(HWND hwnd, HICON icon) {
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

    wcscpy(trayicon_.szTip, Application::getName().c_str());

    Shell_NotifyIconW(NIM_ADD, &trayicon_);
    Shell_NotifyIconW(NIM_SETVERSION, &trayicon_);

    added_ = true;
}

void
TrayIcon::remove() {
  Shell_NotifyIconW(NIM_DELETE, &trayicon_);
}

TrayIcon::~TrayIcon(void) {
  if (added_) {
    remove();
  }
}

void
TrayIcon::setToolTip(Cpp::WStringRef src) {
#if (WINVER < 0x0600)
  trayicon_.uFlags = NIF_TIP;
#else
  trayicon_.uFlags = NIF_TIP | NIF_SHOWTIP;
#endif
  wcscpy(trayicon_.szTip, src.begin());
  Shell_NotifyIconW(NIM_MODIFY, &trayicon_);
}

void
TrayIcon::showBalloon(Cpp::WStringRef title, Cpp::WStringRef msg, IconType icontype) {
#if (WINVER < 0x0600)
  trayicon_.uFlags = NIF_INFO;
#else
  trayicon_.uFlags = NIF_INFO | NIF_SHOWTIP;
#endif
  trayicon_.dwInfoFlags = static_cast<int>(icontype);
  wcscpy(trayicon_.szInfoTitle, title.begin());
  wcscpy(trayicon_.szInfo, msg.begin());

  Shell_NotifyIconW(NIM_MODIFY, &trayicon_);
}


} // namespace Windows
