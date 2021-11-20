#pragma once
#include "variables.h"
bool menu_active = true;

int newfov = 90;
bool triggeractive = false;
uintptr_t buffer;
bool espactive = true;
bool radaractive = false;
bool gdibox = false;
bool bhop = false;
bool noflash = false;
bool norecoil = false;

HDC hdc = NULL;
HANDLE hProc = NULL;
DWORD pId = NULL;
HWND hWnd = NULL;

DWORD localPlayer = NULL;
uintptr_t moduleBase = NULL;
uintptr_t engineModule = NULL;

int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);




