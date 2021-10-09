#pragma once
//Windows imports
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <iostream>
#include <WinUser.h>
#include <Windows.h>
#include <future>

//Local imports
#include "offsets.h"
#include "functions.h"
#include "kiero/kiero.h"
#include "kiero/minhook/include/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"
#define WINDOW_NAME "Densebear Private Cheats"
typedef long(__stdcall* EndScene)(LPDIRECT3DDEVICE9);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
