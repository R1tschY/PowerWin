#include "ConfigFile.h"

#include <windows.h>
#include <boost/lexical_cast.hpp>

////////////////////////////////////////////////////////////////////////////////
//     ConfigFile
////////////////////////////////////////////////////////////////////////////////
bool ConfigFile::existsKey(const wchar_t* section, const wchar_t* key) const {
  const wchar_t* test = L"?#ßüäö§$%&!";

  return getString(section, key, test).compare(test) != 0;
}

std::wstring ConfigFile::getString(const wchar_t* section,
                                   const wchar_t* key,
                                   const wchar_t* default_value) const
{
  wchar_t buffer[255];
  DWORD bytes;

  bytes = GetPrivateProfileStringW(section, key, default_value, buffer, sizeof(buffer), filename_.c_str());
  // TODO: Puffer zu klein -> bytes == sizeof(buffer) - 1

  return std::wstring(buffer);
}

int ConfigFile::getInteger(const wchar_t* section,
                           const wchar_t* key,
                           int default_value) const
{
  return GetPrivateProfileInt(section, key, default_value, filename_.c_str());
}

void ConfigFile::setString(const wchar_t* section,
                           const wchar_t* key,
                           const std::wstring& value)
{
  BOOL success = WritePrivateProfileStringW(section, key, value.c_str(), filename_.c_str());
  if (!success) {
//MESSAGE(L"Fehler bei Schreiben von [%s]->%s: %s", section, key, GetLastWindowsError().c_str());
  }
}

void ConfigFile::setInteger(const wchar_t* section,
                            const wchar_t* key,
                            int value)
{

 BOOL success = WritePrivateProfileStringW(section, key,
      boost::lexical_cast<std::wstring>(value).c_str(), filename_.c_str());
  if (!success) {
//    MESSAGE(L"Fehler bei Schreiben von [%s]->%s: %s", section, key, GetLastWindowsError().c_str());
  }
}

std::vector<std::wstring> ConfigFile::getSections() const {
  wchar_t buffer[2048];
  DWORD bytes;
  std::vector<std::wstring> result;

  bytes = GetPrivateProfileStringW(NULL, NULL, NULL, buffer, sizeof(buffer), filename_.c_str());
  // TODO: Puffer zu klein -> bytes == sizeof(buffer)-2

  return splitStringArray(buffer);
}

std::vector<std::wstring> ConfigFile::getKeys(const wchar_t* section) const {
  wchar_t buffer[2048];
  DWORD bytes;
  std::vector<std::wstring> result;

  bytes = GetPrivateProfileStringW(section, NULL, NULL, buffer, sizeof(buffer), filename_.c_str());
  // TODO: Puffer zu klein -> bytes == sizeof(buffer)-2

  return splitStringArray(buffer);
}

std::vector<std::wstring> ConfigFile::splitStringArray(const wchar_t* p) {
  std::vector<std::wstring> result;
  const wchar_t* section;

  while (*p != 0) {
    section = p;
    do p++; while (*p != 0);
    result.push_back(section);
    p++;
  }

  return std::move(result);
}

ConfigFile::ConfigFile()
{

}

void ConfigFile::loadFromFile(const std::wstring& filename) {
  filename_.assign(filename);
}

