# Memory Reading and Writing - Counter - Strike: Global Offensive
Code is currently incredibly messy and not optimized. This project is likely not going to be updated soon. Reading and writing functions are obsolete as this is an internal DLL hack but was formerly external. In handler.cpp, following functions will be fixed in the future to optimize reading and writing: 

```cpp
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
```
Files are messed up currently. 
