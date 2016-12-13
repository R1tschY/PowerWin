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

#include "module.h"

#include <algorithm>
//#include <boost/algorithm/string.hpp>

template class cpp::registry<PowerWin::Module, PowerWin::ModuleRegistryEntry>;

namespace PowerWin {

//bool Module::getBooleanOption(cpp::wstring_view key, bool default_) {
//  auto opt = options_.find(std::wstring(key));
//  if (opt != options_.end()) {
//    // Schlüssel vorhanden
//
//    std::wstring value = opt->second;
//    boost::to_lower(value);
//    if (boost::equal(value, L"true")) {
//      return true;
//    } else if (boost::equal(value, L"false")) {
//      return false;
//    }
//
//    print(L"Cannot read config key %s\n", key.data());
//  }
//
//  return default_;
//}

} // namespace PowerWin
