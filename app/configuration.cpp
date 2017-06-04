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
#include <cpp-utils/scope.h>
#include <boost/algorithm/string.hpp>
#include <lightports/extra/configfile.h>

#include <QSettings>
#include <QString>

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

// TODO: Convert Framework
template<typename T>
T to(cpp::wstring_view);

template<>
QString to<QString>(cpp::wstring_view str)
{
  return QString::fromWCharArray(str.data(), str.size());
}

// Always use QSettings
class IniConfigurationBackend : public ConfigurationBackend
{
public:
  IniConfigurationBackend(cpp::wstring_view filename)
  : ini_file_(to<QString>(filename), QSettings::IniFormat)
  { }

  bool existsSection(cpp::wstring_view section) override
  {
    return ini_file_.contains(to<QString>(section));
  }

  bool existsKey(cpp::wstring_view section, cpp::wstring_view key) override
  {
    ini_file_.beginGroup(to<QString>(section));
    auto guard = cpp::make_guard([&](){ ini_file_.endGroup(); });
    return ini_file_.contains(to<QString>(key));
  }

  std::wstring readValue(cpp::wstring_view section, cpp::wstring_view key,
    cpp::wstring_view standard) override
  {
    ini_file_.beginGroup(to<QString>(section));
    auto guard = cpp::make_guard([&](){ ini_file_.endGroup(); });
    return ini_file_.value(
        to<QString>(key),
        to<QString>(standard)
    ).toString().toStdWString();
  }

  std::vector<std::wstring> listKeys(cpp::wstring_view section) override
  {
    ini_file_.beginGroup(to<QString>(section));
    auto guard = cpp::make_guard([&](){ ini_file_.endGroup(); });
    const auto keys = ini_file_.childKeys();

    std::vector<std::wstring> result;
    result.reserve(keys.size());
    for (auto& key : keys)
      result.push_back(key.toStdWString());
    return result;
  }

  std::vector<std::wstring> listSections() override
  {
    const auto groups = ini_file_.childGroups();

    std::vector<std::wstring> result;
    result.reserve(groups.size());
    for (auto& group : groups)
      result.push_back(group.toStdWString());
    return result;
  }

private:
  QSettings ini_file_;
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
