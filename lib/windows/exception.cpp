/*
 * exception.cpp
 *
 *  Created on: 04.02.2015
 *      Author: richard
 */

#include "exception.h"

#include "debug.h"

#include <comdef.h>

namespace Windows {

Exception::Exception(DWORD error_code) :
    message_(GetWindowsError(error_code)),
    error_code_(HRESULT_FROM_WIN32(error_code)),
    line_(0),
    file_(nullptr)
{ }

Exception::Exception(HRESULT error_code) :
    message_(GetWindowsError(error_code)),
    error_code_(error_code),
    line_(0),
    file_(nullptr)
{ }

Exception::Exception(DWORD error_code, unsigned int line, const wchar_t* file) :
    message_(GetWindowsError(error_code)),
    error_code_(HRESULT_FROM_WIN32(error_code)),
    line_(line),
    file_(file)
{ }

Exception::Exception(HRESULT error_code, unsigned int line, const wchar_t* file) :
    message_(GetWindowsError(error_code)),
    error_code_(error_code),
    line_(line),
    file_(file)
{ }

Exception::~Exception() { }

} /* namespace Windows */
