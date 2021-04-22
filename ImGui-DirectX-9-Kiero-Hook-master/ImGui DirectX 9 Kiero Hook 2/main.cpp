#include "includes.h"
#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <iostream>
#include <WinUser.h>
#include <Windows.h>

HANDLE hProc = NULL;
DWORD pId = NULL;
HWND hWnd = NULL;
DWORD m_iCrosshairId = 0xB3E4;
DWORD m_localPlayerId = 0xD8C2BC;
DWORD m_iTeamNum = 0xF4;
DWORD m_health = 0x100;
DWORD localPlayer = NULL;
DWORD dwEntityList = 0x4DA3F9C;
DWORD m_bSpotted = 0x93D;
DWORD dwGlowObjectManager = 0x52EC580;
DWORD m_bDormant = 0xED;
DWORD m_iGlowIndex = 0xA438;
DWORD m_vecOrigin = 0x138;
DWORD m_dwBoneMatrix = 0x26A8;
DWORD dwViewMatrix = 0x4D958B4;
DWORD m_bSpottedByMask = 0x980;
DWORD m_clrRender = 0x70;
DWORD model_ambient_min = 0x58C05C;
DWORD dwClientState = 0x588FE4;
DWORD dwClientState_ViewAngles = 0x4D90;
DWORD m_iShotsFired = 0xA390;
DWORD m_aimPunchAngle = 0x302C;
DWORD dwForceJump = 0x524DEDC;
DWORD m_fFlags = 0x104;
DWORD dwMouseEnable = 0xD91E60;
DWORD m_flFlashMaxAlpha = 0xA41C;
uintptr_t moduleBase = NULL;
uintptr_t engineModule = NULL;
int closest;
int fov = 90; bool triggeractive = false; uintptr_t buffer; bool espactive = false;
const int SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN); const int xhairx = SCREEN_WIDTH / 2;
const int SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN); const int xhairy = SCREEN_HEIGHT / 2;

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

uintptr_t geteachplayer(int offset) {
	return readProcessMemory<int>(moduleBase + dwEntityList + offset * 0x10);
}

int getteam(uintptr_t player) {
	return readProcessMemory<int>(player + m_iTeamNum);
}

int getcrosshairID(uintptr_t player) {
	return readProcessMemory<int>(player + m_iCrosshairId);
}

void triggerbot() {
	int crosshairid = getcrosshairID(localPlayer);
	int crosshairteam = getteam(geteachplayer(crosshairid - 1));
	int localteam = getteam(localPlayer);

	HWND openhWnd = GetForegroundWindow();
	if (crosshairid > 0 && crosshairid < 32 && localteam != crosshairteam && openhWnd == hWnd && (RPM<bool>(moduleBase + dwMouseEnable) != 48)) {
		std::cout << "Trigger Bot Activated: Player Detected with ID of: " << crosshairid << "\n";
		mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
		Sleep(0);
		mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
		Sleep(100);
	}
}

void radarcheat() {
	for (int i = 1; i < 64; i++) {
		DWORD dwcurrententity = RPM<DWORD>(moduleBase + dwEntityList + i * 0x10);
		if (dwcurrententity) {
			WPM<bool>(dwcurrententity + m_bSpotted, true);
		}
	}
	Sleep(5);
}


struct glowStructEnemy {
	float red = 1.f;
	float green = 0.f;
	float blue = 0.f;
	float alpha = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;
}glowEnm;

struct ClrRenderer {
	BYTE green, red, blue;
}ClrRender;

struct glowStructLocal {
	float red = 0.f;
	float green = 1.f;
	float blue = 0.f;
	float alpha = 1.f;
	uint8_t padding[8];
	float unknown = 1.f;
	uint8_t padding2[4];
	BYTE renderOccluded = true;
	BYTE renderUnoccluded = false;
	BYTE fullBloom = false;
}glowLocal;
#ifdef _WIN64
#define GWL_WNDPROC GWLP_WNDPROC
#endif

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

EndScene oEndScene = NULL;
WNDPROC oWndProc;
static HWND window = NULL;

void InitImGui(LPDIRECT3DDEVICE9 pDevice)
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);
}

bool init = false; bool visible = false;  
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}

	//if (GetAsyncKeyState(VK_INSERT) & 1) {
	//	visible = !visible;
	//	std::cout << "Toggling \n";
	//}

	if (visible == true || (RPM<bool>(moduleBase + dwMouseEnable) == 48)) {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		
		ImGui::NewFrame();
		ImGui::Begin("Densebear Private Cheats v0.0.1alpha");
		ImGui::Text("Welcome to Densebear's private undetected cheat currently in alpha");
		if (ImGui::Button("Enable Triggerbot [F1]")){
			triggeractive = true;
            std::cout << "Triggerbot active \n";
		}
		if (ImGui::Button("Disable Triggerbot [F1]")) {
			triggeractive = false;
			std::cout << "Trigerbot disabled \n";
		}
		if (ImGui::Button("ESP Glow")) {
			espactive = !espactive;
		}
		ImGui::End();
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
	else {
		
	}
	return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	DWORD wndProcId;
	GetWindowThreadProcessId(handle, &wndProcId);

	if (GetCurrentProcessId() != wndProcId)
		return TRUE; // skip to next window

	window = handle;
	return FALSE; // window found abort search
}

HWND GetProcessWindow()
{
	window = NULL;
	EnumWindows(EnumWindowsCallback, NULL);
	return window;
}

DWORD WINAPI MainThread(HMODULE hModule)
{
	bool attached = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D9) == kiero::Status::Success)
		{
			kiero::bind(42, (void**)& oEndScene, hkEndScene);
			do
				window = GetProcessWindow();
			while (window == NULL);
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWL_WNDPROC, (LONG_PTR)WndProc);
			attached = true;
		}
	} while (!attached);

	AllocConsole();
    FILE* f;

    freopen_s(&f, "CONOUT$", "w", stdout);
    system("color 3");
    std::cout << "Console successfully initiated in debug mode. Extracting module base handle\n";
    moduleBase = (uintptr_t)GetModuleHandle("client.dll");
    engineModule = (uintptr_t)GetModuleHandle("engine.dll");
    std::cout << moduleBase << " | " << engineModule << '\n';
   
    try {
        hWnd = FindWindow(0, "Counter-Strike: Global Offensive");
        if (hWnd == 0) {
            std::cout << "Cannot find window." << std::endl;
            exit(-1);
        }
        GetWindowThreadProcessId(hWnd, &pId);
        hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pId);
    }
    catch (...) {
        MessageBox(NULL, "Error getting process information", "Error", NULL);
    }


	

    while (true) {

	
        localPlayer = *(DWORD*)(moduleBase + m_localPlayerId);
        if ((GetAsyncKeyState(VK_END) & 1) && f != 0)
        {
            writeProcessMemory<int>(90, (localPlayer + 0x332C));
			kiero::shutdown();
            fclose(f);
            FreeConsole();
            FreeLibraryAndExitThread(hModule, NULL);
			return 0x0;
        }
        else if (GetAsyncKeyState(VK_UP) & 1) {
            fov += 5; int health = *(int*)(localPlayer + m_health);
            std::cout << health << " " << fov << '\n';
            writeProcessMemory<int>(fov, (localPlayer + 0x332C));
        }
        else if (GetAsyncKeyState(VK_DOWN) & 1) {
            fov -= 5; int health = *(int*)(localPlayer + m_health);
            std::cout << health << " " << fov << '\n';
            writeProcessMemory<int>(fov, (localPlayer + 0x332C));
        }
        //else if (GetAsyncKeyState(VK_F1) & 1                                ) {
        //    triggeractive = !triggeractive;
        //    if (triggeractive == true) {
        //        std::cout << "Triggerbot active \n";
        //    }
        //    else {
        //        std::cout << "Trigerbot disabled \n";
        //    }
        //}
        if (triggeractive == true) {
            triggerbot();
        }
        
        radarcheat();

		if (espactive == true) {

			uintptr_t dwglowmanager = readProcessMemory<uintptr_t>(moduleBase + dwGlowObjectManager);
			int localteam = readProcessMemory<int>(localPlayer + m_iTeamNum);
			ClrRenderer clrrenderer;
			clrrenderer.blue = 244;
			for (int i = 1; i < 32; i++) {
				uintptr_t dwentity = readProcessMemory<uintptr_t>(moduleBase + dwEntityList + i * 0x10);
				int iglowindex = readProcessMemory<int>(dwentity + m_iGlowIndex);
				int enmhealth = RPM<int>(dwentity + m_health);
				if (enmhealth < 1 || enmhealth > 100) continue;
				int dormant = readProcessMemory<int>(dwentity + m_bDormant);
				if (dormant) continue;
				int entityteam = readProcessMemory<int>(dwentity + m_iTeamNum);


				if (localteam == entityteam) {
					WPM<glowStructLocal>(dwglowmanager + (iglowindex * 0x38) + 0x4, glowLocal);
				}
				else if (localteam != entityteam) {
					WPM<glowStructEnemy>(dwglowmanager + (iglowindex * 0x38) + 0x4, glowEnm);
					WPM<ClrRenderer>(dwentity + m_clrRender, ClrRender);
				}
			}
		}
		//No flash
		WPM<float>(localPlayer + m_flFlashMaxAlpha, 0.0f);
        //Bhop
        int flags = RPM<int>(localPlayer + m_fFlags);
        if (flags & 1) {
            buffer = 5;
        }
        else { buffer = 4; }

        if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
            WPM(moduleBase + dwForceJump, buffer);
        }
    }
	return 0x0;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hMod, 0, nullptr));
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}
