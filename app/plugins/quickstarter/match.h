/// \file result.h

#ifndef APP_PLUGINS_QUICKSTARTER_SOURCES_MATCH_H_
#define APP_PLUGINS_QUICKSTARTER_SOURCES_MATCH_H_

#include <string>
#include <utility>
#include <vector>

namespace QuickStarter {

/// \brief
class Match
{
public:
  using InfoDataEntry = std::pair<std::wstring, std::wstring>;
  using InfoData = std::vector<InfoDataEntry>;
  using InfoType = unsigned;
  enum CommonInfoTypes : unsigned {
    Application,
    Action,
    File
  };

  Match(const std::wstring& title, const std::wstring& description);
  virtual ~Match() = default;

  virtual bool hasInfo(InfoType type) const = 0;
  virtual InfoData getInfo(InfoType type) const = 0;
  virtual void execute() const;

  const std::wstring& getDescription() const
  {
    return description_;
  }

  const std::wstring& getTitle() const
  {
    return title_;
  }

private:
  const std::wstring title_;
  const std::wstring description_;
  // TODO HICON
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_SOURCES_MATCH_H_ */
