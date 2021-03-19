#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include <TlHelp32.h>
#include <stdlib.h>
#include <string>

//MEMORYADDRESSTARGET IS A DYNAMIC MEMORY ALLOCATION UNLIKE GENERAL VALUES IN MOST LOCAL GAMES
HANDLE hProc = NULL;
DWORD pID;
DWORD memoryAddressTarget = 0x00FAFCC0;

//Function takes target name such as csgo.exe 32 bit
bool attachProc(char* procName)
{
	//Entry from processes running hProcSnap snapshot of all proc such as an array
	PROCESSENTRY32 procEntry32;
	procEntry32.dwSize = sizeof(PROCESSENTRY32);
	auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//Snapshot error
	if (hProcSnap == INVALID_HANDLE_VALUE) {
		std::cout << "INVAILD_HANDE_VALUE | ERROR IN CREATING PROCESS SNAPSHOT";
		return false;
	}
	//Loop through all active processes whie there is a next process from the entry entry(procEntry32)
	while (Process32Next(hProcSnap, &procEntry32)) {
		std::cout << procEntry32.szExeFile << std::endl;

		if (!strcmp(procName, procEntry32.szExeFile)) {
			system("CLS");
			std::cout << "[Post Identification Debugging]" << std::endl;
			std::cout << "Found Process " << procEntry32.szExeFile << " | Process ID: " << procEntry32.th32ProcessID << ". Attempting To Hook " << std::endl;
			std::cout << "[Debug: Ensure the given memory allocation for dynamic value : VALUE is valid]" << std::endl;
			//Attempt to open the target gives value to hProc
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
			pID = procEntry32.th32ProcessID;

			if (hProc == NULL) {
				system("CLS");
				std::cout << "Failed to hook or open process. Process value of NULL" << std::endl;	
				exit(-1);
			}

			CloseHandle(hProcSnap);
			return true;

		}
		
	}
	
	system("CLS");
	std::cout << "Failed to find target: " << procName << " in process snapshot. " << std::endl << "Ensure target is runnning and restart program. ";
	CloseHandle(hProcSnap);
	exit(-1);
	return false;


}

void debugWin() {
	std::cout << "Reading Value in DWORD Hexadecimal value : " << memoryAddressTarget << " : Value Changed To 100" << std::endl;
	Sleep(3000);
}

template <class dataType>
void writeProcessMemory(dataType valToWrite, DWORD addressToWrite) 
{

	WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);
	debugWin();
	
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
	SetConsoleTitle("Memory External Injection");
	std::string processtarget = "target_memory.exe";

	std::cout << "Input Injection Target: [0] for defaults" << std::endl;
	std::cin >> processtarget;
	if (processtarget == "0") {
		processtarget = "target_memory.exe";
	}
	else {
		processtarget = processtarget + ".exe";
	}
	system("CLS");
	std::cout << "[F1] Inject [F2] Terminate";
	
	while (true) {
		if (GetAsyncKeyState(VK_F1)) {
			attachProc((char*)processtarget.c_str());
			break;
		}

		if (GetAsyncKeyState(VK_F2)) {
			false;
			exit(-1);
		}
	}

	std::cout << "Changed DWORD Value: Externally Injecting int 4 bytes : value" << std::endl;
	while (true) {
		writeProcessMemory<int>(101, memoryAddressTarget);
	}
	
	
}
