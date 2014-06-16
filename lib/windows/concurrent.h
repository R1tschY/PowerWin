#ifndef CONCURRENT_H
#define CONCURRENT_H

#include "types.h"

namespace Windows {

void executeInThread(const Callback& func);
void executeInMainThread(const Callback& func);

} // namespace Windows

#endif // CONCURRENT_H
