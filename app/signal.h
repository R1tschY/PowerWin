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

#ifndef APP_SIGNAL_H_
#define APP_SIGNAL_H_

#include <boost/signals2/signal.hpp>

namespace PowerWin {

template<typename Signatur>
using Signal = boost::signals2::signal<Signatur>;

template<typename Signatur>
using Slot = typename boost::signals2::signal<Signatur>::slot_type;

using SignalConnection = boost::signals2::connection;
using ScopedSignalConnection = boost::signals2::scoped_connection;

template<typename Signal>
class SignalRegisterInterface
{
public:
  using Slot = typename Signal::slot_type;
  using Signatur = typename Signal::slot_function_type;

  SignalRegisterInterface(Signal& signal)
  : signal_(&signal)
  { }

  ScopedSignalConnection connect(const Slot& subscriber)
  {
    return signal_->connect(subscriber);
  }

private:
  Signal* signal_;
};

} /* namespace PowerWin */

#endif /* APP_SIGNAL_H_ */
