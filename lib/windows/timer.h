#ifndef TIMER_H
#define TIMER_H

#include <windows.h>

namespace Windows {

class Timer {
public:
  Timer();

  void start();
  void stop();
  double getSeconds() const;

private:
  LARGE_INTEGER start_;
  LARGE_INTEGER stop_;
};

} // namespace Windows

#endif // TIMER_H
