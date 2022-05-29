#include "pch.h"
#include "MinHook.h"
#include <direct.h>
#include <string>
#include <iostream>

#pragma comment(lib, "libMinHook.x64.lib")

using namespace std;

string Dump_Path;


typedef long long(WINAPI* ptrMonoLoadData)(const void*, unsigned int, int, unsigned int*, char, char*);
ptrMonoLoadData oriMonoLoadData;
long long WINAPI my_mono_image_open_from_data_with_name(const void* data, unsigned int data_len, int need_copy, unsigned int* status, char a5, char* name) {
	oriMonoLoadData(data, data_len, need_copy, status, a5, name);
	string str_name = name;
	if (str_name.find("Assembly-CSharp.dll") != string::npos) {
		void* data_buffer = malloc(data_len);
		memcpy_s(data_buffer, data_len, data, data_len);
		HANDLE hFile = CreateFileA(Dump_Path.c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (INVALID_HANDLE_VALUE != hFile)
		{
			WriteFile(hFile, data_buffer, data_len, NULL, NULL);
			CloseHandle(hFile);
			std::cout << "导出Dll成功：" << Dump_Path << std::endl;
		}
	}
	return oriMonoLoadData(data, data_len, need_copy, status, a5, name);
}


void OnMonoLoad() {
	std::cout << "Hook：mono-2.0-bdwgc.dll" << std::endl;
	PVOID mono_image_open_from_data_with_name_addr = GetProcAddress(GetModuleHandleA("mono-2.0-bdwgc.dll"), "mono_image_open_from_data_with_name");
	if (mono_image_open_from_data_with_name_addr) {
		if (MH_CreateHook(mono_image_open_from_data_with_name_addr, my_mono_image_open_from_data_with_name, reinterpret_cast<LPVOID*>(&oriMonoLoadData)) != MH_OK) {
			return;
		}
		if (MH_EnableHook(mono_image_open_from_data_with_name_addr) != MH_OK) {
			return;
		}
		std::cout << "Hook mono_image_open_from_data_with_name_addr 成功" << std::endl;
	}
}

typedef HMODULE(WINAPI* ptrLoadLibraryExW)(LPCWSTR, HANDLE, DWORD);
ptrLoadLibraryExW oriLoadLibraryExW;
HMODULE
WINAPI
MyLoadLibraryExW(
	_In_ LPCWSTR lpLibFileName,
	_Reserved_ HANDLE hFile,
	_In_ DWORD dwFlags
) {
	char buf[1000];
	memset(buf, 0, sizeof(buf));
	sprintf_s(buf, "%ws", lpLibFileName);
	string buf_str = buf;
	/*
	if (buf_str.find("Sandrock.exe") != string::npos) {
		return 0;
	}*/
	if (buf_str.find("mono-2.0-bdwgc.dll") != string::npos) {
		HMODULE res = oriLoadLibraryExW(lpLibFileName, hFile, dwFlags);
		OnMonoLoad();
		return res;
	}
	return oriLoadLibraryExW(lpLibFileName, hFile, dwFlags);
}

BOOL HookLoadLibraryW() {
	std::cout << "Hook：KernelBase.dll" << std::endl;
	PVOID LoadLibraryWAddr = GetProcAddress(GetModuleHandleA("KernelBase.dll"), "LoadLibraryExW");
	if (!LoadLibraryWAddr) {
		return false;
	}
	if (MH_CreateHook(LoadLibraryWAddr, MyLoadLibraryExW, reinterpret_cast<LPVOID*>(&oriLoadLibraryExW)) != MH_OK) {
		return false;
	}
	if (MH_EnableHook(LoadLibraryWAddr) != MH_OK) {
		return false;
	}
	std::cout << "Hook LoadlibraryW 成功" << std::endl;
	return true;
}

void Start()
{
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout);
	char buffer[MAX_PATH];
	char* cwd = _getcwd(buffer, MAX_PATH);
	string path = buffer;
	Dump_Path = path + "\\Assembly-CSharp.dll";
	std::cout << "当前程序路径：" << path << std::endl;
	if (MH_Initialize() != MH_OK)
	{
		std::cout << "MH_Initialize Error" << path << std::endl;
		return;
	}
	HookLoadLibraryW();
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Start();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

