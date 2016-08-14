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
