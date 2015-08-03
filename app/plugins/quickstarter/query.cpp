/// \file query.cpp

#include "query.h"

namespace QuickStarter {

Query::Query(Categories categories, const std::wstring& search_string)
: categories_(categories), search_string_(search_string)
{
}

} // namespace QuickStarter
