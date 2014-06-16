// stdafx.h : Includedatei für Standardsystem-Includedateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

#pragma once

#define NTDDI_VERSION 0x05010300 // NTDDI_WINXP
#define _WIN32_WINNT  0x0501     // _WIN32_WINNT_WINXP
#define WINVER        0x0501     // _WIN32_WINNT_WINXP
#define _WIN32_IE     0x0700     // _WIN32_IE_IE70

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Selten verwendete Teile der Windows-Header nicht einbinden.
// Windows-Headerdateien:
#include <windows.h>
