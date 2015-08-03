/// \file applicationmatch.h

#ifndef APP_PLUGINS_QUICKSTARTER_MATCHES_APPLICATIONMATCH_H_
#define APP_PLUGINS_QUICKSTARTER_MATCHES_APPLICATIONMATCH_H_

#include "../match.h"

namespace QuickStarter {

class ApplicationMatch : public Match
{
public:
  ApplicationMatch(
    const std::wstring& title,
    const std::wstring& description,
    const std::wstring& file,
    const std::wstring& command_line,
    const std::wstring& working_directory);

  bool hasInfo(InfoType type) const override;
  InfoData getInfo(InfoType type) const override;

private:
  std::wstring file_;
  std::wstring command_line_;
  std::wstring working_directory_;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_MATCHES_APPLICATIONMATCH_H_ */
