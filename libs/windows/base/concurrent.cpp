#include "concurrent.h"

#include <windows.h>
#include <memory>

#include "../core/debug.h"
#include "../base/charcodecs.h"
#include "../controls/utilwindow.h"

namespace Windows {

static DWORD WINAPI executeInThread_cb(void* data) {
  auto pfunc = std::unique_ptr<Action>(static_cast<Action*>(data));

  try {
    (*pfunc)();
    return 0;

  } catch (std::bad_function_call &error) {
    WIN_CRITICAL(L"executeInThread function call failed: %s",
                 to_wstring(error.what()).c_str());
    return 1;
  }
}

void executeInThread(Action func) {
  Action* pfunc = new Action(func);

  CreateThread(NULL, 0, executeInThread_cb, pfunc, 0, NULL); // TODO error handling
}

void executeInMainThread(Action func) {
  Action* pfunc = new Action(func);

  PostMessageW(getCallbackWindow(), WM_CALLBACK, 0, (LPARAM)pfunc);// TODO error handling
}

} // namespace Windows
