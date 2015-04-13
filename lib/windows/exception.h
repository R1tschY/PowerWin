/*
 * exception.h
 *
 *  Created on: 04.02.2015
 *      Author: richard
 */

#ifndef WINDOWS_EXCEPTION_H_
#define WINDOWS_EXCEPTION_H_

#include <windows.h>
#include <exception>
#include <string>

namespace Windows {

class Exception: public std::exception {
public:
  Exception(DWORD error_code);
  Exception(HRESULT error_code);

  Exception(DWORD error_code, unsigned int line, const wchar_t* file);
  Exception(HRESULT error_code, unsigned int line, const wchar_t* file);

  virtual ~Exception();

  const wchar_t* getMessage() const { return message_.c_str(); }
  HRESULT getHResult() const { return error_code_; }
  unsigned int getErrorCode() const { return HRESULT_CODE(error_code_); }

  int getSourceCodeLine() const { return line_; }
  const wchar_t* getSourceCodeFile() const { return file_; }

private:
  std::wstring message_;
  HRESULT error_code_;

  unsigned int line_;
  const wchar_t* file_;
};

} /* namespace Windows */

#endif /* WINDOWS_EXCEPTION_H_ */
