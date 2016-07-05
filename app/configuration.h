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

#ifndef APP_CONFIGURATION_H_
#define APP_CONFIGURATION_H_

#include <vector>
#include <string>
#include <memory>

#include <cpp-utils/strings/string_view.h>

namespace PowerWin {

struct ConfigurationBackend
{
  virtual ~ConfigurationBackend() = default;

  virtual bool existsSection(cpp::wstring_view section) = 0;
  virtual bool existsKey(cpp::wstring_view section, cpp::wstring_view key) = 0;

  virtual std::wstring readValue(cpp::wstring_view section, cpp::wstring_view key, cpp::wstring_view standard) = 0;

  virtual std::vector<std::wstring> listKeys(cpp::wstring_view section) = 0;
  virtual std::vector<std::wstring> listSections() = 0;
};

class Configuration
{
public:
  Configuration();
  Configuration(std::unique_ptr<ConfigurationBackend> backend);

  void loadIniFile(cpp::wstring_view filename);

  bool existsSection(cpp::wstring_view section) { return backend_->existsSection(section); }
  bool existsKey(cpp::wstring_view section, cpp::wstring_view key) { return backend_->existsKey(section, key); }

  std::wstring readValue(cpp::wstring_view section, cpp::wstring_view key,
    cpp::wstring_view standard) { return backend_->readValue(section, key, standard); }

  bool readBoolean(cpp::wstring_view section, cpp::wstring_view key,
    bool standard);

  std::vector<std::wstring> listKeys(cpp::wstring_view section) { return backend_->listKeys(section); }
  std::vector<std::wstring> listSections() { return backend_->listSections(); }

private:
  std::unique_ptr<ConfigurationBackend> backend_;
};

}
 // namespace PowerWin

#endif /* APP_CONFIGURATION_H_ */
