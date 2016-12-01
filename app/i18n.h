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

#pragma once

#include <cpp-utils/preprocessor.h>
#include "powerwin-config.h"

#if POWERWIN_I18N
# include <boost/locale/message.hpp>
# define _(x) ::boost::locale::gettext(CPP_TO_WIDESTRING(x))
# define N_(...) ::boost::locale::ngettext(CPP_TO_WIDESTRING(s), CPP_TO_WIDESTRING(p), n)
# define P_(c, id) ::boost::locale::pgettext(CPP_TO_WIDESTRING(c), CPP_TO_WIDESTRING(id))
# define NP_(c, s, p, n) ::boost::locale::npgettext(CPP_TO_WIDESTRING(c), CPP_TO_WIDESTRING(s), CPP_TO_WIDESTRING(p), n)
#else
# define _(x) (CPP_TO_WIDESTRING(x))
# define N_(...) (CPP_TO_WIDESTRING(s), CPP_TO_WIDESTRING(p), n)
# define P_(c, id) (CPP_TO_WIDESTRING(c), CPP_TO_WIDESTRING(id))
# define NP_(c, s, p, n) (CPP_TO_WIDESTRING(c), CPP_TO_WIDESTRING(s), CPP_TO_WIDESTRING(p), n)
#endif


