/// \file startmenusource.h

#ifndef APP_PLUGINS_QUICKSTARTER_STARTMENUSOURCE_H_
#define APP_PLUGINS_QUICKSTARTER_STARTMENUSOURCE_H_

#include <SQLiteCpp/SQLiteCpp.h>

#include "../source.h"

namespace QuickStarter {

/// \brief
class StartMenuSource : public Source
{
public:
  StartMenuSource();

  bool canHandleQuery(const Query& query) override;
  std::vector<MatchResult> search(const Query& query) override;

private:
  SQLite::Database db_;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_STARTMENUSOURCE_H_ */
