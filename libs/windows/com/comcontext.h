/// \file comcontext.h

#ifndef HOOKLIB_WINDOWS_COM_COMCONTEXT_H_
#define HOOKLIB_WINDOWS_COM_COMCONTEXT_H_

namespace Windows {

class COMContext
{
public:
  COMContext(unsigned coinit = 0);
  ~COMContext();
};

} // namespace Windows

#endif /* HOOKLIB_WINDOWS_COM_COMCONTEXT_H_ */
