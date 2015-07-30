/// \file comcontext.cpp

#include "comcontext.h"

#include <objbase.h>

namespace Windows {

COMContext::COMContext(unsigned coinit)
{
  CoInitializeEx(nullptr, coinit);
}

COMContext::~COMContext()
{
  CoUninitialize();
}

} // namespace Windows
