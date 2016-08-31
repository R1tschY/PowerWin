/*
 * observermixin.cpp
 *
 *  Created on: 30.08.2016
 *      Author: Richard
 */

#include <app/helper/observermixin.h>

namespace PowerWin {

ObserverMixin::ObserverMixin()
{
  // TODO Auto-generated constructor stub

}

SignalConnection& ObserverMixin::addConnection(SignalConnection&& connection)
{
  connections_.push_back(std::move(connection));
  return connections_.back();
}

} /* namespace PowerWin */
