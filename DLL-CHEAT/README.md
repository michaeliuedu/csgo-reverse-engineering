Important (why DLL-CHEAT project is obsolete) - Please use internal-cheat instead. Do not download this project!
Code is currently incredibly messy and not optimized. This project is likely not going to be updated soon. Reading and writing functions are obsolete as this is an internal dll hack but was formerly external. In handler.cpp, following functions will be fixed in the future to optimize reading and writing if you want to copy or refernce the following templates for you external cheat:

	template<typename T> void WPM(SIZE_T address, T buffer) {
		WriteProcessMemory(hProc, (LPVOID)address, &buffer, sizeof(buffer), NULL);
	}

	template<typename T> T RPM(SIZE_T address) {
		T buffer;
		ReadProcessMemory(hProc, (LPCVOID)address, &buffer, sizeof(T), NULL);
		return buffer;
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
Files are messed up currently. For standalone, VAC-BYPASS is a the loader for the cheat.dll generated. Signatures, NetVars, offsets are not read in realtime yet, still finding solutions. Currently relies on manual updates from Hazedumper for DLL-CHEAT. The only things you need to update for internals are patterns and possibly struct/classes.

# Cheat DLL
This is the cheat project of the repo. This is a sepperate project which requires the solution to be executed and generate a DLL file. Antiviruses may interfere with compilation. Internal logic heavily sourced from an external source. Should not change and has worked exceptionally for the whole of source 1. Unsure for source 2 update however. Use at own risk.
