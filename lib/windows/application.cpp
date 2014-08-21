#include "application.h"

#include <time.h>
#include <Shlobj.h>
#include <Shlwapi.h>

#include "debug.h"
#include "macros.h"

namespace Windows {

static WindowsVersion Application_getWindowsVersion() {
  OSVERSIONINFO wininfo;
  WindowsVersion result;

  wininfo.dwOSVersionInfoSize = sizeof(wininfo);
  GetVersionEx(&wininfo);

  if (wininfo.dwMajorVersion == 5) {
    result = (wininfo.dwMinorVersion==0)?WIN_2000:WIN_XP;
  } else if (wininfo.dwMajorVersion == 6) {
    result = (wininfo.dwMinorVersion==0)?WIN_VISTA:WIN_7;
  } else {
    result = WIN_NEWER;
  }

  if (result == WIN_2000) {
    WIN_ERROR(L"%s", L"Betriebssystem älter als Windows XP.");
  }

  if (result == WIN_NEWER) {
    WIN_WARNING(L"%s", L"Betriebssystem neuer als Windows 7.");
  }

  return result;
}

static std::wstring Application_getPath() {
  wchar_t exefilepath[MAX_PATH];

  GetModuleFileNameW(NULL, exefilepath, sizeof(exefilepath));

  return std::wstring(exefilepath);
}

static void Application_newfailed() {
  _putws(L"Zu wenig Speicher!");
  abort();
}

static void Application_terminate() {
  _putws(L"Nicht aufgefangene Ausnahme!");
  abort();
}

static void Application_unexpected() {
  throw;
}

HINSTANCE Application::instance = 0;
WindowsVersion Application::winversion = Application_getWindowsVersion();
std::wstring Application::path = Application_getPath();
std::wstring Application::name;

Application::Application(const wchar_t* name, HINSTANCE instance) {
  setlocale(LC_ALL, "");
  _tzset();

  mutex_ = CreateMutex(NULL, FALSE, name);
  DWORD error = GetLastError();
  if (!mutex_) {
    WIN_WARNING(L"Konnte Mutex nicht erstellen: %s",
                GetWindowsError(error).c_str());
  }

  is_running_ = (error == ERROR_ALREADY_EXISTS);
  Application::instance = instance;
  Application::name.assign(name);

  std::set_new_handler(Application_newfailed);
  std::set_unexpected(Application_unexpected);
  std::set_terminate(Application_terminate);
}

Application::~Application() {
  if (mutex_) {
    CloseHandle(mutex_);
  }
}

int
Application::run(const Callback& entry) const {
  if (!is_running_) {
    try {
      return entry();
    } catch (const std::exception& e) {
      WIN_ERROR(L"Unerwartete Ausnahme: %s", e.what());
    } catch (...) {
      WIN_ERROR(L"Unerwartete Ausnahme!");
    }
  }

  return 0;
}

bool Application::Is64BitWindows()
{
#ifdef ENV32BIT
  // We can check if the operating system is 64-bit by checking whether
  // we are running under Wow64 (we are 32-bit code). We must check if this
  // function is implemented before we call it, because some older 32-bit
  // versions of kernel32.dll (eg. Windows 2000) don't know about it.
  // See "IsWow64Process", http://msdn.microsoft.com/en-us/library/ms684139.aspx

  typedef BOOL (WINAPI *IsWow64Process_t) (HANDLE, BOOL*);

  auto IsWow64Process = (IsWow64Process_t)
      GetProcAddress(GetModuleHandle(L"kernel32"), "IsWow64Process");
  if (IsWow64Process != nullptr) {
    BOOL isWow64 = false;
    if (IsWow64Process(GetCurrentProcess(), &isWow64)) {
      return isWow64;
    }
  }

  return false;
#else
	return true;
#endif
}


// Utils

std::wstring Application::getExecutableFilename() {
  if (_wpgmptr) {
    return std::wstring(_wpgmptr);
  } else {
    wchar_t result[MAX_PATH];
    GetModuleFileNameW(NULL, result, sizeof(result));
    return std::wstring(result);
  }
}

std::wstring Application::getExecutablePath() {
  wchar_t result[MAX_PATH];
  HMODULE hModule = GetModuleHandleW(NULL);
  GetModuleFileNameW(hModule, result, sizeof(result));
  PathRemoveFileSpecW(result);
  return result;
}

std::wstring Application::getConigPath() {
  wchar_t path[MAX_PATH+1];

  HRESULT hr = SHGetFolderPathW(NULL, CSIDL_APPDATA, NULL, 0, path);
  return std::wstring((hr == S_OK)?path:L"");
}

} // namespace Windows
