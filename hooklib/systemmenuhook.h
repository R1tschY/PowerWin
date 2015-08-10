/*
 * systemmenuhook.h
 *
 *  Created on: 31.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_SYSTEMMENUHOOK_H_
#define HOOKLIB_SYSTEMMENUHOOK_H_

namespace Windows {
class IPCData;
}

namespace SystemMenuHook {

namespace MenuId {
enum MenuId : unsigned {
  AlwaysOnTop = 0x00270,
};
} // namespace MenuId

void activate(const Windows::IPCData&);
void deactivate(const Windows::IPCData&);

} // namespace SystemMenuHook

#endif /* HOOKLIB_SYSTEMMENUHOOK_H_ */
