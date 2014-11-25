#include "concurrent.h"

#include <windows.h>
#include <string>
#include <memory>

#include "debug.h"
#include "charcodecs.h"
#include "utilwindow.h"

namespace Windows {

static DWORD WINAPI executeInThread_cb(void* data) {
  auto pfunc = std::unique_ptr<Callback>(static_cast<Callback*>(data));

  try {
    (*pfunc)();
    return 0;

  } catch (std::bad_function_call &error) {
    WIN_CRITICAL(L"executeInThread function call failed: %s",
                 convertFromUtf8(error.what()).c_str());
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
