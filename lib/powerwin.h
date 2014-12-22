#ifndef POWERWIN_H
#define POWERWIN_H

#include <windows.h>

#ifdef BUILD_DLL
#define DLL_PUBLIC __declspec(dllexport)
#else
#define DLL_PUBLIC __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

DLL_PUBLIC
void CALLBACK FixWindows(HINSTANCE hInstance,
                         HINSTANCE hPrevInstance,
                         LPSTR lpCmdLine,
                         int nCmdShow);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // POWERWIN_H
