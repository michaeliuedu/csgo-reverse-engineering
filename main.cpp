#include "includes.h"
#include "variables.h"

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
		std::cout << "Triggerbot ID: " << crosshairid << "\n";
		mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
		Sleep(150);
		mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);	

	}
}

void radarcheat() {
	for (int i = 1; i < 64; i++) {
		DWORD dwcurrententity = RPM<DWORD>(moduleBase + dwEntityList + i * 0x10);
		if (dwcurrententity) {
			WPM<bool>(dwcurrententity + m_bSpotted, true);
		}
	}
	
}


Vector3 WorldToScreen(const Vector3 pos, view_matrix_t matrix) {
	float _x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	float _y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	float inv_w = 1.f / w;
	_x *= inv_w;
	_y *= inv_w;

	float x = screenX * .5f;
	float y = screenY * .5f;

	x += 0.5f * _x * screenX + 0.5f;
	y -= 0.5f * _y * screenY + 0.5f;

	return { x,y,w };
}

void DrawFilledRect(int x, int y, int w, int h)
{
	RECT rect = { x, y, x + w, y + h };
	FillRect(hdc, &rect, EnemyBrush);
}


void DrawBorderBox(int x, int y, int w, int h, int thickness)
{
	DrawFilledRect(x, y, w, thickness); //Top horiz line
	DrawFilledRect(x, y, thickness, h); //Left vertical line
	DrawFilledRect((x + w), y, thickness, h); //right vertical line
	DrawFilledRect(x, y + h, w + thickness, thickness); //bottom horiz line
	
}

void DrawLine(float StartX, float StartY, float EndX, float EndY)
{
	int a, b = 0;
	HPEN hOPen;
	HPEN hNPen = CreatePen(PS_SOLID, 3, EnemyPen);// penstyle, width, color
	hOPen = (HPEN)SelectObject(hdc, hNPen);
	MoveToEx(hdc, StartX, StartY, NULL); //start
	a = LineTo(hdc, EndX, EndY); //end
	DeleteObject(SelectObject(hdc, hOPen));
}


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


bool init = false; int newfov = 90;
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
		if (!init)
		{
			InitImGui(pDevice);
			init = true;
		}

		int cur_code = (int)((RPM<int>(moduleBase + dwMouseEnable)) % 100);

		bear::Functions func;
		func.gui_callback();

		if (ImGui::CollapsingHeader("Visuals")) {
			if (ImGui::Button("ESP Glow")) {
				espactive = !espactive;
			}
			if (ImGui::Button("Radar Cheat")) {
				radaractive = !radaractive;
			}

			if (ImGui::Button("No Flash")) {
				noflash = !noflash;
			}

			if (ImGui::Button("Snap Boxes [BETA] ")) {
				gdibox = !gdibox;
			}

			if (ImGui::SliderInt("FOV Changer", &newfov, -180, 180)) writeProcessMemory<int>(newfov, (localPlayer + m_iDefaultFOV));


		}
		if (ImGui::CollapsingHeader("Legit")) {
			if (ImGui::Button("Triggerbot")) {
				triggeractive = !triggeractive;

			}
			if (ImGui::Button("Perfect Bhop")) {
				bhop = !bhop;
			}
			if (ImGui::Button("Anti-recoil")) {
				norecoil = !norecoil;
			}
		}

		ImGui::End();
		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	
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
	


	bool gotdata = false; Vector3* viewAngles = NULL; int* iShotsFired = 0; Vector3* aimPunchAngle = NULL; Vector3 oPunch{ 0, 0, 0 };

	localPlayer = *(DWORD*)(moduleBase + dwLocalPlayer);
	viewAngles = (Vector3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);
	iShotsFired = (int*)(localPlayer + m_iShotsFired);
	aimPunchAngle = (Vector3*)(localPlayer + m_aimPunchAngle);
	oPunch = { 0, 0, 0 };

	//If still errors sleep(5000)
	menu_code = (int)((RPM<int>(moduleBase + dwMouseEnable)) % 100);

	while (true) {


		int cur_code = (int)((RPM<int>(moduleBase + dwMouseEnable)) % 100);

		if ((GetAsyncKeyState(VK_END) & 1) && f != 0)
		{
			writeProcessMemory<int>(90, (localPlayer + 0x332C));
			kiero::shutdown();
			fclose(f);
			FreeConsole();
			FreeLibraryAndExitThread(hModule, NULL);
			TerminateProcess(hWnd, 1);
			return 0x0;
		}

		//TODO: MAKE THE END KEY PUBLIC EVEN IF DATA ISNT RECIEVED YET
		if (localPlayer == 0 && gotdata == false) {
			localPlayer = *(DWORD*)(moduleBase + dwLocalPlayer);
			viewAngles = (Vector3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);

			iShotsFired = (int*)(localPlayer + m_iShotsFired);

			aimPunchAngle = (Vector3*)(localPlayer + m_aimPunchAngle);

			oPunch = { 0, 0, 0 };
		}
		else if (localPlayer != 0) {
			gotdata = true;
			if (localPlayer == 0) {
				localPlayer = *(DWORD*)(moduleBase + dwLocalPlayer);
				viewAngles = (Vector3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);

				iShotsFired = (int*)(localPlayer + m_iShotsFired);

				aimPunchAngle = (Vector3*)(localPlayer + m_aimPunchAngle);

				oPunch = { 0, 0, 0 };
			}


			if (triggeractive == true) {
				std::async(triggerbot);
			}

			if (radaractive == true) {
				radarcheat();

			}

			if (espactive == true) {

				view_matrix_t vm = RPM<view_matrix_t>(moduleBase + dwViewMatrix);
				uintptr_t dwglowmanager = readProcessMemory<uintptr_t>(moduleBase + dwGlowObjectManager);
				int localteam = readProcessMemory<int>(localPlayer + m_iTeamNum);
				ClrRenderer clrrenderer;
				clrrenderer.red = 244;

				for (int i = 1; i < 32; i++) {
					uintptr_t dwentity = readProcessMemory<uintptr_t>(moduleBase + dwEntityList + i * 0x10);
					int iglowindex = readProcessMemory<int>(dwentity + m_iGlowIndex);
					int enmhealth = RPM<int>(dwentity + m_iHealth);
					if (enmhealth < 1 || enmhealth > 100) continue;
					int dormant = readProcessMemory<int>(dwentity + m_bDormant);
					if (dormant) continue;
					int entityteam = readProcessMemory<int>(dwentity + m_iTeamNum);

					Vector3 pos = RPM<Vector3>(dwentity + m_vecOrigin);
					Vector3 head;
					head.x = pos.x;
					head.y = pos.y;
					head.z = pos.z + 75.f;
					Vector3 screenpos = WorldToScreen(pos, vm);
					Vector3 screenhead = WorldToScreen(head, vm);
					float height = screenhead.y - screenpos.y;
					float width = height / 2.4f;

					if (localteam == entityteam) {
						WPM<glowStructLocal>(dwglowmanager + (iglowindex * 0x38) + 0x4, glowLocal);
					}
					else if (localteam != entityteam) {
						WPM<glowStructEnemy>(dwglowmanager + (iglowindex * 0x38) + 0x4, glowEnm);
						WPM<ClrRenderer>(dwentity + m_clrRender, clrrenderer);

					}
				}

			}

			if (gdibox == true) {

				view_matrix_t vm = RPM<view_matrix_t>(moduleBase + dwViewMatrix);
				int localteam = readProcessMemory<int>(localPlayer + m_iTeamNum);


				for (int i = 1; i < 32; i++) {
					uintptr_t dwentity = readProcessMemory<uintptr_t>(moduleBase + dwEntityList + i * 0x10);
					int enmhealth = RPM<int>(dwentity + m_iHealth);
					if (enmhealth < 1 || enmhealth > 100) continue;
					int dormant = readProcessMemory<int>(dwentity + m_bDormant);
					if (dormant) continue;
					int entityteam = readProcessMemory<int>(dwentity + m_iTeamNum);

					Vector3 pos = RPM<Vector3>(dwentity + m_vecOrigin);
					Vector3 head;
					head.x = pos.x;
					head.y = pos.y;
					head.z = pos.z + 75.f;
					Vector3 screenpos = WorldToScreen(pos, vm);
					Vector3 screenhead = WorldToScreen(head, vm);
					float height = screenhead.y - screenpos.y;
					float width = height / 2.4f;

					if (localteam == entityteam) {

					}
					else if (localteam != entityteam) {
						if (gdibox == true) {
							DrawBorderBox(screenpos.x - (width / 2), screenpos.y, width, height, 3);
							DrawLine(screenX / 2, screenY, screenpos.x, screenpos.y);
						}

					}
				}
			}
			//No flash
			if (noflash == true) {
				WPM<float>(localPlayer + m_flFlashMaxAlpha, 0.0f);

			}

			//Bhop
			if (bhop == true) {
				int flags = RPM<int>(localPlayer + m_fFlags);
				if (flags & 1) {
					buffer = 5;
				}
				else { buffer = 4; }

				if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
					WPM(moduleBase + dwForceJump, buffer);
				}
			}

			//TODO: TRN INTO ASYNC FUNCTION TO CALL IN TRIGGERBOT FUNCTION FOR NO CHOPPINESS
			Vector3 punchAngle = *aimPunchAngle * 2;
			if (*iShotsFired > 1 && (RPM<int>(localPlayer + m_hActiveWeapon) != 61670213) && norecoil == true ) {
				Vector3 newAngle = *viewAngles + oPunch - punchAngle;
				newAngle.Normalize();
				*viewAngles = newAngle;
			}
			oPunch = punchAngle;
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
