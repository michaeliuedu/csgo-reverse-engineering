# Internal Source Based Cheat (Use densebear-internal Folder Only)
**Cheat code being likely be depreciated soon. I just wanted to release it before source 2 releases. This is not a vaiable public game exploit so if you are looking for that, I suggest lookng elsewhere. This is for learning purposes and reference purposes for those who have similiar endeavors, not for cheaters.**

***This project contains 4 projects, each with a visual studio project. Please read the following notices. The cleanest cheat to use is internal-cheat project.***


- internal-cheat - Recently uploaded internal cheat in folder densebear-internal. Based off of existing source 1 leaks. I recomend checking this out if you want a quick base. If it crashes, please check IDA or other programs for changed patterns/sig locations as its pretty simple and you can often get it from hazedumper or other sources such as Osiris. Again, if it crashes, the code is not broken, it just requires maintainence like all others. Classes and structs may need to be updated as well, check source leaks and forums for this. Special thanks to osiris, various open source contributors, and forum members. Menu UI ported from obsolete DLL-CHEAT project.

***Known Issues:***
- Ennabling Engine Glow will crash the cheat. 7/20/22 - The glow struct or entity enums are likely broken.

**Preview of Basic Features + Menu**
![image](https://user-images.githubusercontent.com/74503398/179643417-30212bdf-5d13-43ea-be46-804e27e945ba.png)


**Loaders**
- VAC-BYPASS - Includes a simple VAC bypassing project. (Works as of 7/12/22 in source 1 this may change as source 2 is released). Requires a DLL in the project base directory to inject.

- densebear-loader - Literally just VAC-BYPASS but ported to IMGUI framework to be more visually appealing. Really messy code and terrible thread functions but it should do. The functions code files are the same and sourced from somewhere else I cannot remember.


**Additional Details. Obsolete files:**
- DLL-CHEAT - Folder contains a terrible mix of internal and external (uses external functions w/o internal interfaces). Do not use this. It is only uploaded for storage! Please read the following section as to why you souldn't use this project!

## Important (why DLL-CHEAT project is obsolete) - Please use internal-cheat instead. Do not download this project!
Code is currently incredibly messy and not optimized. This project is likely not going to be updated soon. Reading and writing functions are obsolete as this is an internal dll hack but was formerly external. In handler.cpp, following functions will be fixed in the future to optimize reading and writing if you want to copy or refernce the following templates for you external cheat:

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
Files are messed up currently. For standalone, VAC-BYPASS is a the loader for the cheat.dll generated. Signatures, NetVars, offsets are not read in realtime yet, still finding solutions. Currently relies on manual updates from Hazedumper for DLL-CHEAT. The only things you need to update for internals are patterns and possibly struct/classes.
