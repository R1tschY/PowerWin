// DesktopEx.cpp : Definiert den Einstiegspunkt für die Anwendung.
//

#include "stdafx.h"

#include "Resource.h"
#include "DesktopEx.h"
#include "DesktopHooks.h"
#include "Plattform.h"

// XP Style
//#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Globale Variablen:
HINSTANCE hInst; // Aktuelle Instanz
HWND window;

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
ATOM		 MyRegisterClass(HINSTANCE hInstance);
BOOL		 InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

void on_startup(HWND window) {
  WinExtra::updateMainWindow(window);
  WinExtra::start();
}

void on_exit() {
  WinExtra::destroy();
}

int app_entry() {
  //  Initailisierung von Comctl32.dll
  /*const INITCOMMONCONTROLSEX icce = {
    sizeof(INITCOMMONCONTROLSEX),
    ICC_STANDARD_CLASSES | ICC_LINK_CLASS | ICC_UPDOWN_CLASS
  };
  if (!InitCommonControlsEx(&icce)) {
    ERROR(L"%s\n", L"Kann 'common controls' nicht initailsieren!");
  }*/

  //  Initailisierung
  MyRegisterClass(hInst);

  window = CreateWindowW(L"PowerWin", L"PowerWin",
    WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInst, NULL);
  if (!window) {
    return FALSE;
  }

//  TrayIcon tray;
//  tray.add(window, LoadIcon(Application::getInstance(), MAKEINTRESOURCE(IDI_SMALL)));

  // Hauptnachrichtenschleife:
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

//  tray.remove();

  return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);

  hInst = hInstance;

  return WinExtra::run(hInst, app_entry);
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEX wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style		= CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc	= WndProc;
  wcex.cbClsExtra	= 0;
  wcex.cbWndExtra	= 0;
  wcex.hInstance	= hInstance;
  wcex.hIcon		= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DESKTOPEX));
  wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName	= 0;
  wcex.lpszClassName	= L"PowerWin";
  wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassEx(&wcex);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch (msg) {
  case WM_CREATE:
    on_startup(hwnd);
    break;

  case WM_DESTROY:
    on_exit();
    PostQuitMessage(0);
    break;

  default:
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
  return 0;  
}
