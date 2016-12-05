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

#include <cpp-utils/assert.h>
#include <cpp-utils/algorithm/length.h>
#include <lightports/core/debugstream.h>

namespace PowerWin {

namespace {

struct Log
{
public:
  constexpr Log() : owning_ref(), ptr() { }

  ~Log()
  {
    ptr = nullptr;
  }

  void set(std::shared_ptr<std::wostream>& stream)
  {
    owning_ref = stream;
    ptr = owning_ref.get();
  }

  void set(std::wostream* stream)
  {
    owning_ref = nullptr;
    ptr = stream;
  }

  std::wostream* get() const { return ptr; }

private:
  std::shared_ptr<std::wostream> owning_ref;
  std::wostream* ptr = nullptr;
};

static Log logs[LogLevel_Max] = {};

static const wchar_t* prefixes[] = {
  L"[INFO] ",
  L"[WARN] ",
  L"[ERR ] "
};
static_assert(cpp::length(prefixes) == LogLevel_Max, "update prefixes array");

void setFallback()
{
  // init shared fallback
  std::shared_ptr<std::wostream> fallback =
    std::make_shared<Windows::DebugOutputStream>();
  for (std::size_t i = 0; i < LogLevel_Max; i++)
  {
    if (logs[i].get())
      continue;

    logs[i].set(fallback);
  }
}

} // namespace

std::wostream& log(LogLevel level)
{
  cpp_assert(level >= 0 && level < LogLevel_Max);

  if (!logs[level].get())
  {
    setFallback();
  }

  return *logs[level].get() << prefixes[level];
}

void setLogStream(LogLevel level, std::shared_ptr<std::wostream> stream)
{
  logs[level].set(stream);

  if (!stream)
  {
    setFallback();
  }
}

void setLogStreams(std::shared_ptr<std::wostream> stream)
{
  for (std::size_t i = 0; i < LogLevel_Max; i++)
  {
    logs[i].set(stream);
  }

  if (!stream)
  {
    setFallback();
  }
}

} // namespace PowerWin

