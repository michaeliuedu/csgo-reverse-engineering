#pragma once
#include <fstream>
#include "includes.h"
#pragma warning(disable:4996)

//Functions mainly from CyborgElf but changed to fit context of program
namespace Functions
{
	bool DoesFileExist(const char* name) {
		if (FILE* file = fopen(name, "r")) {
			fclose(file);
			return true;
		}

		return false;
	}
	
	DWORD GetProcessId(const char* ProcessName)
	{
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(pe32);

		if (!Process32First(hSnap, &pe32))
			return NULL;

		do {

			if (!strcmp(pe32.szExeFile, ProcessName))
			{
				CloseHandle(hSnap);
				return pe32.th32ProcessID;
			}

		} while (Process32Next(hSnap, &pe32));

		CloseHandle(hSnap);
		return NULL;
	}

	uintptr_t GetModuleBaseAddress(DWORD pid, const char* modName) {
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
		if (hSnap != INVALID_HANDLE_VALUE) {
			MODULEENTRY32 modEntry;
			modEntry.dwSize = sizeof(modEntry);
			if (Module32First(hSnap, &modEntry)) {
				do {
					if (!strcmp(modEntry.szModule, modName)) {
						CloseHandle(hSnap);
						return (uintptr_t)modEntry.modBaseAddr;
					}
				} while (Module32Next(hSnap, &modEntry));
			}
		}
	}

	bool LoadLibraryInject(DWORD ProcessId, const char* Dll)
	{
		if (ProcessId == NULL)
			return false;

		char CustomDLL[MAX_PATH];
		GetFullPathName(Dll, MAX_PATH, CustomDLL, 0);

		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
		LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(CustomDLL), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		if (!WriteProcessMemory(hProcess, allocatedMem, CustomDLL, sizeof(CustomDLL), NULL))
			return FALSE;

		CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, allocatedMem, 0, 0);

		if (hProcess)
			CloseHandle(hProcess);

		return TRUE;
	}

	
	
	namespace Internal
	{
		LPVOID NTOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");

		bool ExecuteBypass(HANDLE hProcess)
		{
			if (NTOpenFile) {
				char originalBytes[5];
				memcpy(originalBytes, NTOpenFile, 5);
				if (WriteProcessMemory(hProcess, NTOpenFile, originalBytes, 5, NULL)) {
					std::cout << "VAC Bypassed.." << std::endl;
					return TRUE;
				}

			}

			return FALSE;

		}

		bool Backup(HANDLE hProcess)
		{
			if (NTOpenFile) {
				char Orig[5];
				memcpy(Orig, NTOpenFile, 5);
				WriteProcessMemory(hProcess, NTOpenFile, Orig, 0, 0);
				return TRUE;
			}

			return FALSE;
		}

	}
}
