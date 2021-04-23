#include "includes.h"
#include <windows.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <iostream>
#include <WinUser.h>
#include <Windows.h>
#include <future>

#define EnemyPen 0x000000FF
HDC hdc = NULL;
HBRUSH EnemyBrush = CreateSolidBrush(0x000000FF);
HANDLE hProc = NULL;
DWORD pId = NULL;
HWND hWnd = NULL;
DWORD localPlayer = NULL;

//DWORD m_iCrosshairId = 0xB3E4;
//DWORD m_localPlayerId = 0xD8C2BC;
//DWORD m_iTeamNum = 0xF4;
//DWORD m_health = 0x100;
//DWORD dwEntityList = 0x4DA3F9C;
//DWORD m_bSpotted = 0x93D;
//DWORD dwGlowObjectManager = 0x52EC580;
//DWORD m_bDormant = 0xED;
//DWORD m_iGlowIndex = 0xA438;
//DWORD m_vecOrigin = 0x138;
//DWORD m_dwBoneMatrix = 0x26A8;
//DWORD dwViewMatrix = 0x4D958B4;
//DWORD m_bSpottedByMask = 0x980;
//DWORD m_clrRender = 0x70;
//DWORD model_ambient_min = 0x58C05C;
//DWORD dwClientState = 0x588FE4;
//DWORD dwClientState_ViewAngles = 0x4D90;
//DWORD m_iShotsFired = 0xA390;
//DWORD m_aimPunchAngle = 0x302C;
//DWORD dwForceJump = 0x524DEDC;
//DWORD m_fFlags = 0x104;
//DWORD dwMouseEnable = 0xD91E60;
//DWORD m_flFlashMaxAlpha = 0xA41C;
//DWORD m_iDefaultFOV = 0x332C;

DWORD cs_gamerules_data = 0x0;
DWORD m_ArmorValue = 0xB378;
DWORD m_Collision = 0x320;
DWORD m_CollisionGroup = 0x474;
DWORD m_Local = 0x2FBC;
DWORD m_MoveType = 0x25C;
DWORD m_OriginalOwnerXuidHigh = 0x31C4;
DWORD m_OriginalOwnerXuidLow = 0x31C0;
DWORD m_SurvivalGameRuleDecisionTypes = 0x1328;
DWORD m_SurvivalRules = 0xD00;
DWORD m_aimPunchAngle = 0x302C;
DWORD m_aimPunchAngleVel = 0x3038;
DWORD m_angEyeAnglesX = 0xB37C;
DWORD m_angEyeAnglesY = 0xB380;
DWORD m_bBombDefused = 0x29B0;
DWORD m_bBombPlanted = 0x9A5;
DWORD m_bBombTicking = 0x2980;
DWORD m_bFreezePeriod = 0x20;
DWORD m_bGunGameImmunity = 0x3944;
DWORD m_bHasDefuser = 0xB388;
DWORD m_bHasHelmet = 0xB36C;
DWORD m_bInReload = 0x32A5;
DWORD m_bIsDefusing = 0x3930;
DWORD m_bIsQueuedMatchmaking = 0x74;
DWORD m_bIsScoped = 0x3928;
DWORD m_bIsValveDS = 0x7C;
DWORD m_bSpotted = 0x93D;
DWORD m_bSpottedByMask = 0x980;
DWORD m_bStartedArming = 0x33F0;
DWORD m_bUseCustomAutoExposureMax = 0x9D9;
DWORD m_bUseCustomAutoExposureMin = 0x9D8;
DWORD m_bUseCustomBloomScale = 0x9DA;
DWORD m_clrRender = 0x70;
DWORD m_dwBoneMatrix = 0x26A8;
DWORD m_fAccuracyPenalty = 0x3330;
DWORD m_fFlags = 0x104;
DWORD m_flC4Blow = 0x2990;
DWORD m_flCustomAutoExposureMax = 0x9E0;
DWORD m_flCustomAutoExposureMin = 0x9DC;
DWORD m_flCustomBloomScale = 0x9E4;
DWORD m_flDefuseCountDown = 0x29AC;
DWORD m_flDefuseLength = 0x29A8;
DWORD m_flFallbackWear = 0x31D0;
DWORD m_flFlashDuration = 0xA420;
DWORD m_flFlashMaxAlpha = 0xA41C;
DWORD m_flLastBoneSetupTime = 0x2924;
DWORD m_flLowerBodyYawTarget = 0x3A90;
DWORD m_flNextAttack = 0x2D70;
DWORD m_flNextPrimaryAttack = 0x3238;
DWORD m_flSimulationTime = 0x268;
DWORD m_flTimerLength = 0x2994;
DWORD m_hActiveWeapon = 0x2EF8;
DWORD m_hBombDefuser = 0x29B4;
DWORD m_hMyWeapons = 0x2DF8;
DWORD m_hObserverTarget = 0x338C;
DWORD m_hOwner = 0x29CC;
DWORD m_hOwnerEntity = 0x14C;
DWORD m_hViewModel = 0x32F8;
DWORD m_iAccountID = 0x2FC8;
DWORD m_iClip1 = 0x3264;
DWORD m_iCompetitiveRanking = 0x1A84;
DWORD m_iCompetitiveWins = 0x1B88;
DWORD m_iCrosshairId = 0xB3E4;
DWORD m_iDefaultFOV = 0x332C;
DWORD m_iEntityQuality = 0x2FAC;
DWORD m_iFOVStart = 0x31E8;
DWORD m_iGlowIndex = 0xA438;
DWORD m_iHealth = 0x100;
DWORD m_iItemDefinitionIndex = 0x2FAA;
DWORD m_iItemIDHigh = 0x2FC0;
DWORD m_iMostRecentModelBoneCounter = 0x2690;
DWORD m_iObserverMode = 0x3378;
DWORD m_iShotsFired = 0xA390;
DWORD m_iState = 0x3258;
DWORD m_iTeamNum = 0xF4;
DWORD m_lifeState = 0x25F;
DWORD m_nBombSite = 0x2984;
DWORD m_nFallbackPaintKit = 0x31C8;
DWORD m_nFallbackSeed = 0x31CC;
DWORD m_nFallbackStatTrak = 0x31D4;
DWORD m_nForceBone = 0x268C;
DWORD m_nTickBase = 0x3430;
DWORD m_nViewModelIndex = 0x29C0;
DWORD m_rgflCoordinateFrame = 0x444;
DWORD m_szCustomName = 0x303C;
DWORD m_szLastPlaceName = 0x35B4;
DWORD m_thirdPersonViewAngles = 0x31D8;
DWORD m_vecOrigin = 0x138;
DWORD m_vecVelocity = 0x114;
DWORD m_vecViewOffset = 0x108;
DWORD m_viewPunchAngle = 0x3020;
DWORD m_zoomLevel = 0x33D0;

DWORD anim_overlays = 0x2980;
DWORD clientstate_choked_commands = 0x4D30;
DWORD clientstate_delta_ticks = 0x174;
DWORD clientstate_last_outgoing_command = 0x4D2C;
DWORD clientstate_net_channel = 0x9C;
DWORD convar_name_hash_table = 0x2F0F8;
DWORD dwClientState = 0x588FE4;
DWORD dwClientState_GetLocalPlayer = 0x180;
DWORD dwClientState_IsHLTV = 0x4D48;
DWORD dwClientState_Map = 0x28C;
DWORD dwClientState_MapDirectory = 0x188;
DWORD dwClientState_MaxPlayer = 0x388;
DWORD dwClientState_PlayerInfo = 0x52C0;
DWORD dwClientState_State = 0x108;
DWORD dwClientState_ViewAngles = 0x4D90;
DWORD dwEntityList = 0x4DA3F9C;
DWORD dwForceAttack = 0x31D54DC;
DWORD dwForceAttack2 = 0x31D54E8;
DWORD dwForceBackward = 0x31D5530;
DWORD dwForceForward = 0x31D553C;
DWORD dwForceJump = 0x524DEDC;
DWORD dwForceLeft = 0x31D5554;
DWORD dwForceRight = 0x31D5548;
DWORD dwGameDir = 0x6277F8;
DWORD dwGameRulesProxy = 0x52C11BC;
DWORD dwGetAllClasses = 0xDB3F8C;
DWORD dwGlobalVars = 0x588CE8;
DWORD dwGlowObjectManager = 0x52EC580;
DWORD dwInput = 0x51F5560;
DWORD dwInterfaceLinkList = 0x946F44;
DWORD dwLocalPlayer = 0xD8C2BC;
DWORD dwMouseEnable = 0xD91E60;
DWORD dwMouseEnablePtr = 0xD91E30;
DWORD dwPlayerResource = 0x31D3850;
DWORD dwRadarBase = 0x51D8D14;
DWORD dwSensitivity = 0xD91CFC;
DWORD dwSensitivityPtr = 0xD91CD0;
DWORD dwSetClanTag = 0x8A1B0;
DWORD dwViewMatrix = 0x4D958B4;
DWORD dwWeaponTable = 0x51F6020;
DWORD dwWeaponTableIndex = 0x325C;
DWORD dwYawPtr = 0xD91AC0;
DWORD dwZoomSensitivityRatioPtr = 0xD96D60;
DWORD dwbSendPackets = 0xD749A;
DWORD dwppDirect3DDevice9 = 0xA7050;
DWORD find_hud_element = 0x2D4FE320;
DWORD force_update_spectator_glow = 0x3AE412;
DWORD interface_engine_cvar = 0x3E9EC;
DWORD is_c4_owner = 0x3BAF00;
DWORD m_bDormant = 0xED;
DWORD m_flSpawnTime = 0xA370;
DWORD m_pStudioHdr = 0x294C;
DWORD m_pitchClassPtr = 0x51D8FB0;
DWORD m_yawClassPtr = 0xD91AC0;
DWORD model_ambient_min = 0x58C05C;
DWORD set_abs_angles = 0x1DF9C0;
DWORD set_abs_origin = 0x1DF800;

uintptr_t moduleBase = NULL;
uintptr_t engineModule = NULL;

int closest;
bool triggeractive = false; uintptr_t buffer; bool espactive = false; bool radaractive = false; bool gdibox = false; bool bhop = false; bool noflash = false; 
bool norecoil = false;
const int screenX = GetSystemMetrics(SM_CXSCREEN); 
const int screenY = GetSystemMetrics(SM_CYSCREEN);
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

struct view_matrix_t {
	float* operator[](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};

struct Vector3 {
	float x, y, z;

	Vector3 operator+(Vector3 d) {
		return{ x + d.x, y + d.y, z + d.z };
	}
	Vector3 operator-(Vector3 d) {
		return{ x - d.x, y - d.y, z - d.z };
	}

	Vector3 operator*(float d) {
		return { x * d, y * d, z * d };
	}

	void Normalize() {
		while (y < -180) {
			y += 360;
		} while (y > 180) {
			y -= 360;
		}
		if (x > 89) {
			x = 89;
		}
		if (x < -89) {
			x = -89;
		}
	}
};


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

bool init = false; int newfov = 90;
long __stdcall hkEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	if (!init)
	{
		InitImGui(pDevice);
		init = true;
	}

	
	if ((RPM<bool>(moduleBase + dwMouseEnable) == 48)) {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Densebear Private Cheats v0.0.1alpha");
		ImGui::Text("Welcome to Densebear's private undetected cheat currently in alpha");
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

			if(ImGui::SliderInt("FOV Changer", &newfov, -180, 180)) writeProcessMemory<int>(newfov, (localPlayer + m_iDefaultFOV));

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

	Vector3* viewAngles = (Vector3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);

	int* iShotsFired = (int*)(localPlayer + m_iShotsFired);

	Vector3* aimPunchAngle = (Vector3*)(localPlayer + m_aimPunchAngle);

	Vector3 oPunch{ 0, 0, 0 };


	while (true) {


		if ((GetAsyncKeyState(VK_END) & 1) && f != 0)
		{
			writeProcessMemory<int>(90, (localPlayer + 0x332C));
			kiero::shutdown();
			fclose(f);
			FreeConsole();
			FreeLibraryAndExitThread(hModule, NULL);
			return 0x0;
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


		//Recoil TODO:KEEP ADDIGN  PISTOL IDS ONLY USP ADDED WE NEED GLOCK DEAGLE AND REV AND P-WHATEVER IDK
		Vector3 punchAngle = *aimPunchAngle * 2;
		if (*iShotsFired > 0 && (RPM<int>(localPlayer + m_hActiveWeapon) != 61670213) && norecoil == true) {
			Vector3 newAngle = *viewAngles + oPunch - punchAngle;
			newAngle.Normalize();
			*viewAngles = newAngle;
		}
		oPunch = punchAngle;
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
