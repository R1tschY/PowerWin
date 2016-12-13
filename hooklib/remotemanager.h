///
/// Copyright (c) 2016 R1tschY
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to
/// deal in the Software without restriction, including without limitation the
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#ifndef HOOKLIB_REMOTEMANAGER_H_
#define HOOKLIB_REMOTEMANAGER_H_

#include <memory>
#include <vector>
#include <lightports/controls/messagesink.h>

#include "macros.h"
#include "hookmodule.h"

namespace PowerWin {

class RemoteManager : public Windows::MessageSink {
public:
  RemoteManager();

  void activate();
  void deactivate();

  void onCreate() override;
  void onDestroy() override;
  LRESULT onMessage(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
  std::vector<std::unique_ptr<HookModule>> modules_;
  Windows::Window app_hwnd_ = nullptr;
  bool activated_ = false;
};

extern "C" {

DLL_PUBLIC
void CALLBACK EnterGodModus(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow);

} // extern "C"

} // namespace PowerWin

#endif /* HOOKLIB_REMOTEMANAGER_H_ */
