// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <iostream>

HANDLE hProc = NULL;
DWORD pId = NULL;
HWND hWnd = NULL;   

template <class dataType>
void writeProcessMemory(dataType valToWrite, DWORD addressToWrite)
{
    WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);

}

DWORD WINAPI HackThread(HMODULE hModule) {
    //File f is the console
    AllocConsole();
    FILE* f;

    freopen_s(&f, "CONOUT$", "w", stdout);
    system("color 3");
    std::cout << "Console successfully initiated in debug mode. Extracting module base handle\n";
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle("client.dll");
    std::cout << moduleBase << '\n';
    //modulebase is the exe or dll file of the game file defined under uintptr_t size (DWORD works too)
   
    try {
        hWnd = FindWindow(0, "Counter-Strike: Global Offensive");
        if (hWnd == 0) {
            std::cout << "Cannot find window." << std::endl;
        }
        GetWindowThreadProcessId(hWnd, &pId);
        hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
    }
    catch (...) {
        MessageBox(NULL, "Error getting process information", "Error", NULL);
    }

    bool fovtoggle = false;
    //Handling toggling and termination
    while (true) {
        if (GetAsyncKeyState(VK_END) & 1)
        {
            fclose(f);
            FreeConsole();
            FreeLibraryAndExitThread(hModule, NULL);
        }
        else if (GetAsyncKeyState(VK_F1) & 1){
            fovtoggle = !fovtoggle;
            if (fovtoggle == true) {
                printf("Information Activated : Player Health and FOV \n");
            }
            else {
                printf("Information Deactivated : Player Health and FOV \n");
            }
        } 
        else if (GetAsyncKeyState(VK_F4) & 1) {
            if (fovtoggle == true) {
                DWORD localPlayer = *(DWORD*)(moduleBase + 0xD8C2BC);
                printf("Defaults Reset\n"); fovtoggle = false;
                writeProcessMemory<int>(90, (localPlayer + 0x332C));
            }
        }
        if (fovtoggle == true) {
            DWORD localPlayer = *(DWORD*)(moduleBase + 0xD8C2BC);
            int health = *(int*)(localPlayer + 0x100);
            int fov = *(int*)(localPlayer + 0x332C);
            std::cout << health <<" " << fov << '\n';
            writeProcessMemory<int>(144, (localPlayer + 0x332C));
            system("CLS");
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
    case DLL_PROCESS_ATTACH: {
        MessageBox(NULL, "Injection Successful into Counter-Strike : Global Offensive", "Closing thread creation process. Proceeding with hack thread", NULL);
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

