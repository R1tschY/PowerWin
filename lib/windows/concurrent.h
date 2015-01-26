#ifndef CONCURRENT_H
#define CONCURRENT_H

#include <functional>

namespace Windows {

typedef std::function<void()> Action;

void executeInThread(Action func);
void executeInMainThread(Action func);

} // namespace Windows

#endif // CONCURRENT_H
