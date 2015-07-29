#pragma once

#include <string>
#include <vector>

#include "macros.h"
#include <c++/stringview.h>

namespace Windows {

////////////////////////////////////////////////////////////////////////////////
//     ConfigFile
////////////////////////////////////////////////////////////////////////////////
class ConfigFile {
  DISALLOW_COPY_AND_ASSIGN(ConfigFile);

public:  
  ConfigFile();

  void loadFromFile(cpp::wstring_view filename);
  void save() { }

  bool existsKey(cpp::wstring_view section, cpp::wstring_view key) const;

  std::wstring getString(cpp::wstring_view section, cpp::wstring_view key, cpp::wstring_view default_value) const;
  int getInteger(cpp::wstring_view section, cpp::wstring_view key, int default_value) const;
  void setString(cpp::wstring_view section, cpp::wstring_view key, const std::wstring& value);
  void setInteger(cpp::wstring_view section, cpp::wstring_view key, int value);

  std::vector<std::wstring> getSections() const;
  std::vector<std::wstring> getKeys(cpp::wstring_view section) const;

private:
  std::wstring filename_;

  static std::vector<std::wstring> splitStringArray(const wchar_t* strings);
};

} // namespace Windows
