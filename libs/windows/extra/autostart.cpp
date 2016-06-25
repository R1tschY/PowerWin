#include "autostart.h"

#include "../core/debug.h"
#include "../base/application.h"
#include <windows.h>

namespace Windows {

bool setProgramToAutostart(bool value) {
  HKEY key;
  LONG error = RegCreateKeyExW (
    HKEY_CURRENT_USER,
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
    0, NULL, REG_OPTION_NON_VOLATILE,
    KEY_ALL_ACCESS, NULL, &key, 0);
  win_throw_on_fail(error);

  if (value) {
    error = RegSetValueExW(key, Application::getName().c_str(), 0, REG_SZ,
      (LPBYTE)(Application::getExecutablePath().c_str()), (DWORD)((Application::getExecutablePath().size() + 1) * sizeof(wchar_t)));
  } else {
    if (RegQueryValueExW(key, Application::getName().c_str(),0,0,0,0) != ERROR_FILE_NOT_FOUND)
      error = RegDeleteValueW(key, Application::getName().c_str());
  }

  RegCloseKey(key); // TODO: use RAII
  win_throw_on_fail(error);
  return true;
}

bool isProgramInAutostart() {
  HKEY key;
  LONG error;

  error = RegCreateKeyExW(
    HKEY_CURRENT_USER,
    L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
    0, NULL, REG_OPTION_NON_VOLATILE,
    KEY_ALL_ACCESS, NULL, &key, 0);
  win_throw_on_fail(error);


  DWORD buffersize = 0;

#if (WINVER < _WIN32_WINNT_VISTA)
  DWORD type;
  error = RegQueryValueEx(key, Application::getName().c_str(), NULL,  &type, NULL, &buffersize);
#else
  error = RegGetValue(key, NULL, Application::getName().c_str(), RRF_RT_REG_SZ, NULL, NULL, &buffersize);
#endif

  if (error != ERROR_SUCCESS) {
    if (error != ERROR_FILE_NOT_FOUND) {
      WIN_WARNING(L"cannot read autostart registry value: %d", error);
    }
    RegCloseKey(key); // TODO: use RAII
    return false;
  }
#if (WINVER < _WIN32_WINNT_VISTA)
  if (type != REG_SZ) {
    WIN_WARNING(L"%s", L"autostart registry value has wrong type.");
    RegCloseKey(key); // TODO: use RAII
    return false;
  }
#endif

  wchar_t buffer[buffersize+1];
#if (WINVER < _WIN32_WINNT_VISTA)
  error = RegQueryValueEx(key, Application::getName().c_str(), NULL,  &type, (BYTE*)buffer, &buffersize);
#else
  error = RegGetValue(key, NULL, Application::getName().c_str(), RRF_RT_REG_SZ, NULL, buffer, &buffersize);
#endif

  if (error != ERROR_SUCCESS) {
    if (error != ERROR_FILE_NOT_FOUND) {
      win_throw_on_fail(error);
    }
    RegCloseKey(key); // TODO: use RAII
    return false;
  } else {
    RegCloseKey(key); // TODO: use RAII
    return (Application::getExecutablePath().compare(buffer) == 0);
  }
}

} // namespace Windows
