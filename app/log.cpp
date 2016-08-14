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

#include "log.h"

#include <algorithm>

#include <lightports/core/debugstream.h>


namespace PowerWin {

void Log::setDevice(std::wstreambuf* buffer)
{
  this->set_rdbuf(buffer);
}

void Log::setDevice(std::unique_ptr<std::wstreambuf> buffer)
{
  buffer_ = std::move(buffer);

  this->rdbuf(buffer_.get());
}

void Log::initGlobalChannels()
{
  // init all logs
  getInfoLog();
  getWarningsLog();
  getCriticalLog();
}

std::wostream& Log::info()
{
  return getInfoLog() << L"INFO: ";
}

std::wostream& Log::warning()
{
  return getWarningsLog() << L"WARNING: ";
}

std::wostream& Log::critical()
{
  return getCriticalLog() << L"CRITICAL: ";
}

Log& Log::getInfoLog()
{
  static Log instance{new Windows::DebugOutputBuffer()};
  return instance;
}

Log& Log::getWarningsLog()
{
  static Log instance{new Windows::DebugOutputBuffer()};
  return instance;
}

Log& Log::getCriticalLog()
{
  static Log instance{new Windows::DebugOutputBuffer()};
  return instance;
}

} // namespace PowerWin

