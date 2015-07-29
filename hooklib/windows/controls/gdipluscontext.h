#ifndef GDIPLUSCONTEXT_H
#define GDIPLUSCONTEXT_H

#include <windows.h>

namespace Windows {

class GdiplusContext
{
public:
  GdiplusContext();
  ~GdiplusContext();

private:
  ULONG_PTR token_;
};

} // namespace Windows

#endif // GDIPLUSCONTEXT_H
