#include "includes.h"
#include "variables.h"
#include "handler.h"
#include "threadhook.h"
#include "modules.h"

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


bool init = false; 
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
		if (!init)
		{
			InitImGui(pDevice);
			init = true;
		}

		if (menu_active) {
			POINT point;
			if (GetCursorPos(&point)) {
			}
			Thread::gui_callback();
		}
		
	
	return oEndScene(pDevice);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

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

	ShowWindow(GetConsoleWindow(), SW_HIDE);
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
		hdc = GetDC(FindWindowA(NULL, "Counter-Strike: Global Offensive"));
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


	localPlayer = *(DWORD*)(moduleBase + dwLocalPlayer);

	while (true) {

		BYTE cur_code = (BYTE)(( Handler::RPM<BYTE>(moduleBase + dwMouseEnable)));
		if ((GetAsyncKeyState(VK_END) & 1) && f != 0)
		{
			Handler::writeProcessMemory<int>(90, (localPlayer + 0x332C));
			kiero::shutdown();
			fclose(f);
			FreeConsole();
			FreeLibraryAndExitThread(hModule, NULL);
			TerminateProcess(hWnd, 1);
			return 0x0;
		}

		if ((GetAsyncKeyState(VK_F1))) {
			menu_active = !menu_active;
		}

	
		Modules::recoil();
		Modules::triggerbot();
		Modules::glow_esp();
		Modules::snap_boxes();
		Modules::no_flash();
		Modules::auto_bhop();
		Modules::radar_cheat();

			
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
