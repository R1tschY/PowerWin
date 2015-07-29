#include <windows.h>

#include "powerwin.h"

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
  FixWindows(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
  return 0;
}


