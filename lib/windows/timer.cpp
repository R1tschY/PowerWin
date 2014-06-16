#include "timer.h"

#include "debug.h"

namespace Windows {

static double getFrequency() {
  LARGE_INTEGER proc_freq;

  if (!QueryPerformanceFrequency (&proc_freq))
    WIN_ERROR(L"%s", L"Aufruf von QueryPerformanceFrequency() gescheitert");

  return 1.0/((double)proc_freq.QuadPart);
}

const double Timer::Frequency = getFrequency();

Timer::Timer() {
  start();
}
Timer::~Timer() {}

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
  return (stop_.QuadPart - start_.QuadPart) * Frequency;
}

}


