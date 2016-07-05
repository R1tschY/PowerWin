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

#include "configuration.h"

#include <cpp-utils/assert.h>
#include <cpp-utils/strings/string_literal.h>
#include <boost/algorithm/string.hpp>
#include <lightports/base/configfile.h>

namespace PowerWin {

namespace {
// NullConfigurationBackend

struct NullConfigurationBackend : public ConfigurationBackend
{
  bool existsSection(cpp::wstring_view section) override
  {
    return false;
  }

  bool existsKey(cpp::wstring_view section, cpp::wstring_view key) override
  {
    return false;
  }

  std::wstring readValue(cpp::wstring_view section, cpp::wstring_view key,
    cpp::wstring_view standard) override
  {
    return standard.to_string();
  }

  std::vector<std::wstring> listKeys(cpp::wstring_view section) override
  {
    return { };
  }

  std::vector<std::wstring> listSections() override
  {
    return { };
  }
};

// IniConfigurationBackend

class IniConfigurationBackend : public ConfigurationBackend
{
public:
  IniConfigurationBackend(cpp::wstring_view filename)
  {
    ini_file_.loadFromFile(filename);
  }

  bool existsSection(cpp::wstring_view section) override
  {
    return false; // TODO: add support
  }

  bool existsKey(cpp::wstring_view section, cpp::wstring_view key) override
  {
    return ini_file_.existsKey(section, key);
  }

  std::wstring readValue(cpp::wstring_view section, cpp::wstring_view key,
    cpp::wstring_view standard) override
  {
    return ini_file_.getString(section, key, standard);
  }

  std::vector<std::wstring> listKeys(cpp::wstring_view section) override
  {
    return ini_file_.getKeys(section);
  }

  std::vector<std::wstring> listSections() override
  {
    return ini_file_.getSections();
  }

private:
  Windows::ConfigFile ini_file_;
};
}

//

Configuration::Configuration()
: backend_(std::make_unique<NullConfigurationBackend>())
{
}

Configuration::Configuration(std::unique_ptr<ConfigurationBackend> backend)
: backend_(backend ? std::move(backend) : std::make_unique<NullConfigurationBackend>())
{ }

void Configuration::loadIniFile(cpp::wstring_view filename)
{
  backend_ = std::make_unique<IniConfigurationBackend>(filename);
}

bool Configuration::readBoolean(cpp::wstring_view section, cpp::wstring_view key,
  bool standard)
{
  using namespace cpp::string_view_literals;

  std::wstring value = readValue(section, key, standard ? L"1"sv : L"0"sv);

  boost::to_lower(value);
  boost::trim(value);

  return (value == L"1"sv || value == L"true"sv || value == L"yes"sv);
}

} // namespace PowerWin
