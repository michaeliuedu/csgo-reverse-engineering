#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>

using namespace std;

void get_proc_id(const char* window_title, DWORD& process_id) {
	GetWindowThreadProcessId(FindWindow(NULL, window_title), &process_id);
}

void error(const char* error_title, const char* error_message) {
	MessageBox(NULL, error_message, error_title, NULL);
	exit(-1);
}

bool file_exists(string file_name) {
	struct stat buffer;
	return (stat(file_name.c_str(), &buffer) == 0);
}

int main() {

	DWORD proc_id = NULL;
	char dll_path[MAX_PATH];
	const char* dll_name = "THROWDLL.dll";
	const char* window_title = "AssaultCube";
	string raw_window; bool message_active = false; int runtime = 0;

	system("color 4");
	std::cout << "Input window title that you would like to inject message into (x86 architecture) [0] For Assault Cube [1] For CS:GO" << "\n";
	std::getline(std::cin, raw_window);
	system("CLS");

	if (raw_window == "0") {
		raw_window = window_title;
		dll_name = "THROWDLL.dll";
		std::cout << "Attmpting to inject DLL into Assault Cube";
	}
	else if (raw_window == "1") {
		window_title = "Counter-Strike: Global Offensive";
		dll_name = "internalcsgodll.dll";
		std:cout << "Attmpting to inject DLL into CS:GO";
	}
	else {
		window_title = raw_window.c_str();
		std::cout << "Attmpting to inject DLL into " + raw_window;
	}
	

	if (!file_exists(dll_name)) {
		error("File_Exists", "File doesnt exist in current scope");
	}

	if (!GetFullPathName(dll_name, MAX_PATH, dll_path, nullptr)) {
		error("GetFullPathName", "Failed to aquire full path");
	}

	get_proc_id(window_title, proc_id);
	if (proc_id == NULL) error("get_proc_id", "Failed to aquire process ID");

	HANDLE h_process = OpenProcess(PROCESS_ALL_ACCESS, NULL, proc_id);
	if (!h_process) {
		error("OpenProcess", "Failed to open a handle to process");
	}

	void* alloc_memory = VirtualAllocEx(h_process, nullptr, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!alloc_memory) {
		error("VirtualAllocEx", "Failed to allocate memory");
		exit(-1);
	}
	if (!h_process) {
		error("h_processNULL", "Failed to find handle process");
		exit(-1);
	}

	if (!WriteProcessMemory(h_process, alloc_memory, dll_path, MAX_PATH, nullptr)) {
		error("WriteProcessMemory", "Failed to write process memory");
	}

	HANDLE h_thread = CreateRemoteThread(h_process, nullptr, NULL, LPTHREAD_START_ROUTINE(LoadLibraryA), alloc_memory, NULL, nullptr);
	if (!h_thread) {
		error("CreateRemoteThread", "Failed to create remote thread");
	}

	ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
	while (true) {
		std::cout << "Successful Injection : DLL Running for Runtime : " << runtime << " seconds" << "\n";
		Sleep(1000);
		system("cls");
		runtime += 1;
	}

	CloseHandle(h_process);
	VirtualFreeEx(h_process, alloc_memory, NULL, MEM_RELEASE);
	//MessageBox(0, "Injection Success", "Success", 0);

	return 0;
}