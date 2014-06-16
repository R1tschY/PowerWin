#ifndef TIMER_H
#define TIMER_H

#include <windows.h>

namespace Windows {

class Timer {
public:
  Timer();
  ~Timer();

  void start();
  void stop();
  double getSeconds() const;

private:
  LARGE_INTEGER start_;
  LARGE_INTEGER stop_;

  static const double Frequency;
};

} // namespace Windows

#endif // TIMER_H
