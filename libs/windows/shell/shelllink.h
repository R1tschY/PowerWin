/*
 * shelllink.h
 *
 *  Created on: 30.07.2015
 *      Author: richard
 */

#ifndef HOOKLIB_WINDOWS_COM_SHELLLINK_H_
#define HOOKLIB_WINDOWS_COM_SHELLLINK_H_

#include <windows.h>
#include "../com/comcontext.h"
#include "../com/comptr.h"
#include "../extra/shortcut.h"

class IShellLinkW;
class IPersistFile;

namespace Windows {

class ShellLink
{
public:
  ShellLink();
  ~ShellLink();

  ShellLink(const ShellLink&) = delete;
  ShellLink& operator=(const ShellLink&) = delete;

  // IPersistFile::Load
  void open(const std::wstring& path, unsigned mode = 0 /* STGM_READ */);

  // IShellLink::Resolve
  void resolve(unsigned timeout_ms = 3000, unsigned flags = 0);
  void resolve(HWND window, unsigned flags);

  // IShellLink::GetArguments
  void getArguments(std::wstring& value) const;

  // IShellLink::GetDescription
  void getDescription(std::wstring& value) const;

  // IShellLink::GetHotkey
  void getHotkey(ShortCut& value) const;

  // IShellLink::GetIconLocation
  void getIconLocation(std::wstring& path, int& index) const;

  // IShellLink::GetPath
  void getPath(std::wstring& value, unsigned flags = 0) const;

  // IShellLink::GetShowCmd
  void getShowCmd(int& value) const;

  // IShellLink::GetWorkingDirectory
  void getWorkingDirectory(std::wstring& value) const;

  // internals
  IShellLinkW* getIShellLink() const { return shell_link_.get(); }

private:
  COMPtr<IShellLinkW> shell_link_;
  COMContext context_;
};

} /* namespace Windows */

#endif /* HOOKLIB_WINDOWS_COM_SHELLLINK_H_ */
