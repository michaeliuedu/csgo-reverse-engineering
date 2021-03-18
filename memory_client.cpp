#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <stdlib.h>

HANDLE hProc = NULL;
DWORD pID;

//Function takes target name such as csgo.exe 32 bit
bool attachProc(char* procName)
{
	//Entry from processes running hProcSnap snapshot of all proc such as an array
	PROCESSENTRY32 procEntry32;
	procEntry32.dwSize = sizeof(PROCESSENTRY32);
	auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//Snapshot error
	if (hProcSnap == INVALID_HANDLE_VALUE) {
		std::cout << "INVAILD_HANDE_VALUE";
		return false;
	}
	//Loop through all active processes whie there is a next process from the entry entry(procEntry32)
	while (Process32Next(hProcSnap, &procEntry32)) {
		std::cout << procEntry32.szExeFile << std::endl;

		if (!strcmp(procName, procEntry32.szExeFile)) {
			std::cout << "Found Process " << procEntry32.szExeFile << " | Process ID: " << procEntry32.th32ProcessID;
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ParentProcessID);
			pID = procEntry32.th32ProcessID;

			if (hProc == NULL) {
				std::cout << "Failed to hook or open process. hProc == NULL" << std::endl;	
			}

			CloseHandle(hProcSnap);
			return true;


		}
		
	}
	std::cout << "Failed to find target: " << procName << " in process snapshot";
	CloseHandle(hProcSnap);
	return false;

}

template <class dataType>
void writeProcessMemory(dataType valToWrite, DWORD addressToWrite) 
{
	WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);
}

template <class dataType>
dataType readProcessMemory(DWORD addressToRead) {
	dataType rpmBuffer;
	ReadProcessMemory(hProc, (PVOID)addressToRead, &rpmBuffer, sizeof(dataType), 0);

	return rpmBuffer;
}

int main()
{
	system("Color 4");
	DWORD memoryAddressTarget = 0x00CFFBE8;
	attachProc((char*)"target_memory.exe");

	while (true) {
		writeProcessMemory<int>(100, memoryAddressTarget);
	}
}
