#ifndef APP_SIGNAL_H_
#define APP_SIGNAL_H_

#include <boost/signals2/signal.hpp>

namespace PowerWin {

template<typename Signatur>
using Signal = boost::signals2::signal<Signatur>;

template<typename Signatur>
using Slot = typename boost::signals2::signal<Signatur>::slot_type;

using SignalConnection = boost::signals2::connection;

template<typename Signal>
class SignalRegisterInterface
{
public:
  using Slot = typename Signal::slot_type;
  using Signatur = typename Signal::slot_function_type;

  SignalRegisterInterface(Signal& signal)
  : signal_(&signal)
  { }

  SignalConnection connect(const Slot& subscriber)
  {
    return signal_->connect(subscriber);
  }

private:
  Signal* signal_;
};

} /* namespace PowerWin */

#endif /* APP_SIGNAL_H_ */
