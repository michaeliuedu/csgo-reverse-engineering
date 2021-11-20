#pragma once
#include "variables.h"
#include "handler.h"
#include "offsets.h"
#include "includes.h"

#pragma warning(disable:4996)
bool gotdata = false; Handler::Components::Vector3* viewAngles = NULL; int* iShotsFired = 0; Handler::Components::Vector3* aimPunchAngle = NULL; Handler::Components::Vector3 oPunch{ 0, 0, 0 };

namespace Modules
{
	void no_flash() {
		if (noflash) {
			Handler::WPM<float>(localPlayer + m_flFlashMaxAlpha, 0.0f);

		}
	}

	void auto_bhop() {
		if (bhop) {
			int flags = Handler::RPM<int>(localPlayer + m_fFlags);
			if (flags & 1) {
				buffer = 5;
			}
			else { buffer = 4; }

			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				Handler::WPM(moduleBase + dwForceJump, buffer);
			}
		}

	}

	void radar_cheat() {
		if (radaractive) {
			for (int i = 1; i < 64; i++) {
				DWORD dwcurrententity = Handler::RPM<DWORD>(moduleBase + dwEntityList + i * 0x10);
				if (dwcurrententity) {
					Handler::WPM<bool>(dwcurrententity + m_bSpotted, true);
				}
			}
		}
	}

	void snap_boxes() {
		if (gdibox == true) {

			Handler::Components::view_matrix_t vm = Handler::RPM<Handler::Components::view_matrix_t>(moduleBase + dwViewMatrix);
			int localteam = Handler::readProcessMemory<int>(localPlayer + m_iTeamNum);


			for (int i = 1; i < 32; i++) {
				uintptr_t dwentity = Handler::readProcessMemory<uintptr_t>(moduleBase + dwEntityList + i * 0x10);
				int enmhealth = Handler::RPM<int>(dwentity + m_iHealth);
				if (enmhealth < 1 || enmhealth > 100) continue;
				int dormant = Handler::readProcessMemory<int>(dwentity + m_bDormant);
				if (dormant) continue;
				int entityteam = Handler::readProcessMemory<int>(dwentity + m_iTeamNum);

				Handler::Components::Vector3 pos = Handler::RPM<Handler::Components::Vector3>(dwentity + m_vecOrigin);
				Handler::Components::Vector3 head;
				head.x = pos.x;
				head.y = pos.y;
				head.z = pos.z + 75.f;
				Handler::Components::Vector3 screenpos = screenpos.WorldToScreen(pos, vm);
				Handler::Components::Vector3 screenhead = screenpos.WorldToScreen(head, vm);
				float height = screenhead.y - screenpos.y;
				float width = height / 2.4f;

				if (localteam == entityteam) {

				}
				else if (localteam != entityteam) {
					if (gdibox == true) {
						Handler::Util::Rendering::DrawBorderBox(screenpos.x - (width / 2), screenpos.y, width, height, 3);
						Handler::Util::Rendering::DrawLine(screenX / 2, screenY, screenpos.x, screenpos.y);
					}

				}
			}
		}
	}

	void glow_esp() {
		if (espactive == true) {

			uintptr_t dwglowmanager = Handler::readProcessMemory<uintptr_t>(moduleBase + dwGlowObjectManager);
			int localteam = Handler::readProcessMemory<int>(localPlayer + m_iTeamNum);
			Handler::Components::ClrRenderer clrrenderer;
			clrrenderer.red = 244;

			for (int i = 1; i < 32; i++) {
				uintptr_t dwentity = Handler::readProcessMemory<uintptr_t>(moduleBase + dwEntityList + i * 0x10);
				int iglowindex = Handler::readProcessMemory<int>(dwentity + m_iGlowIndex);
				int enmhealth = Handler::RPM<int>(dwentity + m_iHealth);
				if (enmhealth < 1 || enmhealth > 100) continue;
				if (Handler::readProcessMemory<int>(dwentity + m_bDormant)) continue;
				int entityteam = Handler::readProcessMemory<int>(dwentity + m_iTeamNum);

				if (localteam == entityteam) {
					Handler::WPM<Handler::Components::glowStructLocal>(dwglowmanager + (iglowindex * 0x38) + 0x8, Handler::Components::glowLocal);
				}
				else if (localteam != entityteam) {
					Handler::WPM<Handler::Components::glowStructEnemy>(dwglowmanager + (iglowindex * 0x38) + 0x8, Handler::Components::glowEnm);
					Handler::WPM<Handler::Components::ClrRenderer>(dwentity + m_clrRender, clrrenderer);

				}
			}

		}
	}
	
	void recoil () {
		if (localPlayer == 0 && gotdata == false) {
			localPlayer = *(DWORD*)(moduleBase + dwLocalPlayer);
			viewAngles = (Handler::Components::Vector3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);
			iShotsFired = (int*)(localPlayer + m_iShotsFired);
			aimPunchAngle = (Handler::Components::Vector3*)(localPlayer + m_aimPunchAngle);
			oPunch = { 0, 0, 0 };
		}

		else if (localPlayer != 0) {
			gotdata = true;
			if (localPlayer == 0) {
				localPlayer = *(DWORD*)(moduleBase + dwLocalPlayer);
				viewAngles = (Handler::Components::Vector3*)(*(uintptr_t*)(engineModule + dwClientState) + dwClientState_ViewAngles);
				iShotsFired = (int*)(localPlayer + m_iShotsFired);
				aimPunchAngle = (Handler::Components::Vector3*)(localPlayer + m_aimPunchAngle);
				oPunch = { 0, 0, 0 };
			}
			Handler::Components::Vector3 punchAngle = *aimPunchAngle * 2;
			if (*iShotsFired > 1 && norecoil == true) {
				Handler::Components::Vector3 newAngle = *viewAngles + oPunch - punchAngle;
				newAngle.Normalize();
				*viewAngles = newAngle;
			}
			oPunch = punchAngle;
		}

	}

	void triggerbot() {
		if (triggeractive) {
			int crosshairid = Handler::Util::getcrosshairID(localPlayer);
			int crosshairteam = Handler::Util::getteam(Handler::Util::geteachplayer(crosshairid - 1));
			int localteam = Handler::Util::getteam(localPlayer);

			HWND openhWnd = GetForegroundWindow();
			if (crosshairid > 0 && crosshairid < 32 && localteam != crosshairteam && openhWnd == hWnd && (Handler::RPM<bool>(moduleBase + dwMouseEnable) != 48)) {
				std::cout << "Triggerbot ID: " << crosshairid << "\n";
				mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
				Sleep(150);
				mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);

			}
		}
	}



}