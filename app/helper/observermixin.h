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

#ifndef APP_HELPER_OBSERVERMIXIN_H_
#define APP_HELPER_OBSERVERMIXIN_H_

#include <app/signals.h>

namespace PowerWin {

class ObserverMixin
{
public:
  ObserverMixin();

  template<typename Signal>
  SignalConnection& connect(
    SignalRegisterInterface<Signal> signal,
    const typename SignalRegisterInterface<Signal>::Slot& slot)
  {
    return addConnection(signal.connect(slot));
  }

private:
  std::vector<ScopedSignalConnection> connections_;

  SignalConnection& addConnection(SignalConnection&& connection);
};

} /* namespace PowerWin */

#endif /* APP_HELPER_OBSERVERMIXIN_H_ */
