///
/// Copyright (c) 2016 R1tschY
/// 
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to 
/// deal in the Software without restriction, including without limitation the 
/// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
/// sell copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
/// 
/// The above copyright notice and this permission notice shall be included in
/// all copies or substantial portions of the Software.
/// 
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
/// IN THE SOFTWARE.
///

#ifndef APP_LOG_H_
#define APP_LOG_H_

#include <ostream>

namespace PowerWin {

class Log final : public std::wostream
{
public:
  Log(std::wstreambuf* buffer) :
    std::wostream(buffer),
    buffer_()
  { }

  Log(std::unique_ptr<std::wstreambuf> buffer) :
    std::wostream(buffer.get()),
    buffer_(std::move(buffer))
  { }

  void setDevice(std::wstreambuf* buffer);
  void setDevice(std::unique_ptr<std::wstreambuf> buffer);

  void useDeviceOf(const std::wostream& stream) { setDevice(stream.rdbuf()); }

  static void initGlobalChannels();

  static Log& getInfoLog();
  static Log& getWarningsLog();
  static Log& getCriticalLog();

  static std::wostream& info();
  static std::wostream& warning();
  static std::wostream& critical();

private:
  std::unique_ptr<std::wstreambuf> buffer_;
};

} // namespace PowerWin

#endif /* APP_LOG_H_ */
