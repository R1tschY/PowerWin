/*
 * observermixin.h
 *
 *  Created on: 30.08.2016
 *      Author: Richard
 */

#ifndef APP_HELPER_OBSERVERMIXIN_H_
#define APP_HELPER_OBSERVERMIXIN_H_

#include "../signal.h"

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
