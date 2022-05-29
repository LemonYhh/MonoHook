#pragma once
#include <shlwapi.h>
DWORD _stdcall GetFileVersionInfoA();
DWORD _stdcall GetFileVersionInfoByHandle();
DWORD _stdcall GetFileVersionInfoExA();
DWORD _stdcall GetFileVersionInfoExW();
DWORD _stdcall GetFileVersionInfoSizeA();
DWORD _stdcall GetFileVersionInfoSizeExA();
DWORD _stdcall GetFileVersionInfoSizeExW();
DWORD _stdcall GetFileVersionInfoSizeW();
DWORD _stdcall GetFileVersionInfoW();
DWORD _stdcall VerFindFileA();
DWORD _stdcall VerFindFileW();
DWORD _stdcall VerInstallFileA();
DWORD _stdcall VerInstallFileW();
DWORD _stdcall VerLanguageNameA();
DWORD _stdcall VerLanguageNameW();
DWORD _stdcall VerQueryValueA();
DWORD _stdcall VerQueryValueW();