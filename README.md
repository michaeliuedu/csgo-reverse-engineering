# Internal Source Based Cheat
**Cheat code being likely be depreciated soon. I just wanted to release it before source 2 releases. This is not a vaiable public game exploit so if you are looking for that, I suggest lookng elsewhere. This is for learning purposes and reference purposes for those who have similiar endeavors, not for cheaters.**


- internal-cheat - Recently uploaded internal cheat in folder densebear-internal. Based off of existing source 1 leaks. I recommend checking this out if you want a quick base. If it crashes, please check IDA or other programs for changed patterns/sig locations as its pretty simple and you can often get it from hazedumper or other sources such as Osiris. **Again, if it crashes, the code is not broken, it just requires maintainence like all other existing cheats**, especially considering that I will stop updating these myself for a while. Classes and structs may need to be updated as well, check source leaks and forums for this. Special thanks to osiris, various open source contributors, and forum members. Menu UI ported from obsolete DLL-CHEAT project.

***Known Issues:***
- Enabling Engine Glow will crash the cheat. 7/20/22 - The glow struct or entity enums are likely broken.

**Preview of Basic Features + Menu**
![image](https://user-images.githubusercontent.com/74503398/179643417-30212bdf-5d13-43ea-be46-804e27e945ba.png)


**Loaders**
- VAC-BYPASS - Includes a simple VAC bypassing project. (Works as of 7/12/22 in source 1 this may change as source 2 is released). Requires a DLL in the project base directory to inject.


Files are messed up currently. For standalone, VAC-BYPASS is a the loader for the cheat.dll generated. Signatures, NetVars, offsets are not read in realtime yet, still finding solutions. Currently relies on manual updates from Hazedumper for DLL-CHEAT. The only things you need to update for internals are patterns and possibly struct/classes.
