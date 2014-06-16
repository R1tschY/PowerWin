#include "concurrent.h"

#include <windows.h>
#include <string>

#include "debug.h"
#include "charcodecs.h"
#include "utilwindow.h"

namespace Windows {

static DWORD WINAPI executeInThread_cb(LPVOID data) {
  Callback* pfunc = static_cast<Callback*>(data);

  try {
    (*pfunc)();
    delete pfunc;
    return 0;

  } catch (std::bad_function_call &error) {
    WIN_CRITICAL(L"executeInThread function call failed: %s",
                 convertFromUtf8(error.what()).c_str());
    if (pfunc) delete pfunc;
    return 1;
  }
}

void executeInThread(const Callback& func) {
  Callback* pfunc = new Callback(func);

  CreateThread(NULL, 0, executeInThread_cb, pfunc, 0, NULL); // TODO Fehlerbehandlung
}

void executeInMainThread(const Callback& func) {
  Callback* pfunc = new Callback(func);

  PostMessageW(getCallbackWindow(), WM_CALLBACK, 0, (LPARAM)pfunc);// TODO Fehlerbehandlung
}

} // namespace Windows
