/*
 * shelllink.cpp
 *
 *  Created on: 30.07.2015
 *      Author: richard
 */

#include "shelllink.h"

#include <shobjidl.h>
#include <shlguid.h>
#include <shobjidl.h>
#include <cassert>
#include <c++/utils.h>
#include "../debug.h"
#include "../bits.h"

namespace Windows {

ShellLink::ShellLink()
{
  // create
  IShellLinkW* shell_link = nullptr;
  win_throw_on_fail(
    CoCreateInstance(
      CLSID_ShellLink,
      nullptr,
      CLSCTX_INPROC_SERVER,
      IID_IShellLink,
      reinterpret_cast<void**>(&shell_link)
    )
  );
  shell_link_.reset(shell_link);
}

ShellLink::~ShellLink() { }

void ShellLink::open(const std::wstring& path, unsigned mode)
{
  COMPtr<IPersistFile> persist_file;

  IPersistFile* tmp;
  win_throw_on_fail(
    shell_link_->QueryInterface(
      IID_IPersistFile,
      reinterpret_cast<void**>(&tmp)
    )
  );
  persist_file.reset(tmp);

  // open file
  win_throw_on_fail(
    persist_file->Load(path.c_str(), mode)
  );
}

void ShellLink::resolve(unsigned timeout_ms, unsigned flags)
{
  assert(shell_link_ != nullptr);

  high_word(flags, timeout_ms);

  win_throw_on_fail(
    shell_link_->Resolve(nullptr, SLR_NO_UI | flags)
  );
}

void ShellLink::resolve(HWND window, unsigned flags)
{
  assert(shell_link_ != nullptr);

  win_throw_on_fail(
    shell_link_->Resolve(window, flags)
  );
}

void ShellLink::getArguments(std::wstring& value) const
{
  assert(shell_link_ != nullptr);

  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetArguments(dest, cpp::length(dest))
  );
  value.assign(dest);
}

void ShellLink::getDescription(std::wstring& value) const
{
  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetDescription(dest, cpp::length(dest))
  );
  value.assign(dest);
}

void ShellLink::getHotkey(ShortCut& value) const
{
  uint16_t result;
  win_throw_on_fail(
    shell_link_->GetHotkey(&result)
  );
  uint8_t mod = high_byte(result);

  value.key = low_byte(result);
  value.modifiers = 0;
  if (mod & HOTKEYF_SHIFT) value.modifiers |= MOD_SHIFT;
  if (mod & HOTKEYF_CONTROL) value.modifiers |= MOD_CONTROL;
  if (mod & HOTKEYF_ALT) value.modifiers |= MOD_ALT;
  if (mod & HOTKEYF_EXT) value.modifiers |= MOD_WIN;
}

void ShellLink::getIconLocation(std::wstring& path, int& index) const
{
  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetIconLocation(dest, cpp::length(dest), &index)
  );
  path.assign(dest);
}

void ShellLink::getPath(std::wstring& value, unsigned flags) const
{
  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetPath(dest, cpp::length(dest), nullptr, flags)
  );
  value.assign(dest);
}

void ShellLink::getShowCmd(int& value) const
{
  win_throw_on_fail(
    shell_link_->GetShowCmd(&value)
  );
}

void ShellLink::getWorkingDirectory(std::wstring& value) const
{
  wchar_t dest[MAX_PATH];
  win_throw_on_fail(
    shell_link_->GetWorkingDirectory(dest, cpp::length(dest))
  );
  value.assign(dest);
}

} /* namespace Windows */
