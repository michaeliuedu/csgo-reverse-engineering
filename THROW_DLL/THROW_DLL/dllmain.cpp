// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <iostream>

DWORD WINAPI HackThread(HMODULE hModule) {
    //File f is the console
    AllocConsole();
    FILE* f;

    //Player Base: "ac_client.exe"+0x109B74/509B74 Player Health : 0xF8 
    //offsets at:lnk.https://www.mpgh.net/forum/showthread.php?t=664311
    freopen_s(&f, "CONOUT$", "w", stdout);
    system("color 3");
    std::cout << "Console successfully initiated in debug mode. Extracting module base handle\n";
    std::cout << "Commands: [F1] Regeneration [F2] Armor [F3] Rapid [END] Exit \n";
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle("ac_client.exe");
    DWORD localPlayer = *(DWORD*)(moduleBase + 0x10F4F4);

    //modulebase is the exe or dll file of the game file defined under uintptr_t size (DWORD works too)

    bool toggleHealth = false, toggleArmor = false, Rapid = false, toggleESP = false;

    //Handling toggling and termination
    while (true) {
        if (GetAsyncKeyState(VK_F1) & 1)
        {
            toggleHealth = !toggleHealth;
            if (toggleHealth == true){
                printf("Health Toggled Active [F1]\n");

            }else{
                printf("Health Toggled Disabled [F1]\n");
            }

        }

        else if (GetAsyncKeyState(VK_F2) & 1)
        {
            toggleArmor = !toggleArmor;
            if (toggleArmor == true) {
                printf("Armor Toggled Active [F2]\n");

            }
            else {
                printf("Armor Toggled Disabled [F2]\n");
            }

        } else if (GetAsyncKeyState(VK_F3) & 1)
        {
            Rapid = !Rapid;
            if (Rapid == true) {
                printf("Rapid Toggled Active [F3]\n");

            }
            else {
                printf("Rapid Toggled Disabled [F3]\n");
            }

        }

        if (Rapid == true) {
            //Primary
            *(int*)(localPlayer + 0x128) = 9999;
            *(int*)(localPlayer + 0x150) = 9999;
            //Secondary
            *(int*)(localPlayer + 0x13C) = 9999;
            *(int*)(localPlayer + 0x158) = 9999;
            //Gernade
            *(int*)(localPlayer + 0x158) = 9999;
            *(int*)(localPlayer + 0x180) = 0;
            *(int*)(localPlayer + 0x178) = 0;
            *(int*)(localPlayer + 0x160) = 0;

        }

        if (toggleArmor == true) {
             *(int*)(localPlayer + 0xFC) = 9999;
        }

        if (toggleHealth == true) {
            *(int*)(localPlayer + 0xF8) = 9999;
        }

        if (GetAsyncKeyState(VK_END) & 1)
        {
            fclose(f);
            FreeConsole();
            FreeLibraryAndExitThread(hModule, NULL);
        }

    }

    return 0x0;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        MessageBox(NULL, "Injection Successful into Assault Cube Client Process", "Closing thread creation process. Proceeding with hack thread", NULL);
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

