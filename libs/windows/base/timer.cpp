#include "timer.h"

#include "../core/debug.h"

namespace Windows {

static double computeFrequency() {
  LARGE_INTEGER proc_freq;

  if (!QueryPerformanceFrequency (&proc_freq)) {
    WIN_ERROR(L"%s", L"QueryPerformanceFrequency failed!");
    return NAN;
  }

  return 1.0/((double)proc_freq.QuadPart);
}

static double getFrequency() {
  static const double frequency = computeFrequency();
  return frequency;
}


Timer::Timer() {
  start();
}

void Timer::start() {
  DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);

  QueryPerformanceCounter(&start_);

  SetThreadAffinityMask(GetCurrentThread(), oldmask);
}

void Timer::stop() {
  DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);

  QueryPerformanceCounter(&stop_);

  SetThreadAffinityMask(GetCurrentThread(), oldmask);
}

double Timer::getSeconds() const {
  return (stop_.QuadPart - start_.QuadPart) * getFrequency();
}

}


