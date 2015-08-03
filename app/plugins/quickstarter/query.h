/// \file query.h

#ifndef APP_PLUGINS_QUICKSTARTER_SOURCES_QUERY_H_
#define APP_PLUGINS_QUICKSTARTER_SOURCES_QUERY_H_

#include <string>

namespace QuickStarter {

/// \brief
class Query
{
public:
  enum class Categories {
    Action   = 1 << 0,
    App      = 1 << 1,
    Location = 1 << 2,
    Web      = 1 << 3,
    File     = 1 << 4,

    All      = 0xFFFF
  };

  Query(Categories categories, const std::wstring& search_string);

  Categories getCategories() const { return categories_; }

  bool hasCategories(Categories value) const
  {
    return static_cast<int>(categories_) & static_cast<int>(value);
  }

  const std::wstring& getSearchString() const { return search_string_; }
  // Matcher/*List<Regex, MatchScore>*/ getRegexMatchers() const;
  // Matcher/*List<Regex, MatchScore>*/ getSqlMatchers() const;

private:
  const Categories categories_;
  const std::wstring search_string_;
};

} // namespace QuickStarter

#endif /* APP_PLUGINS_QUICKSTARTER_SOURCES_QUERY_H_ */
