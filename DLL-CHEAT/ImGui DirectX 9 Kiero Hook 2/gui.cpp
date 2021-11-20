#pragma once
#include "variables.h"
#include "handler.h"
#include "offsets.h"
#include "gui.h"

#pragma warning(disable:4996)


namespace Thread
{

	void gui_callback() {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Densebear Private Cheats v0.0.1 alpha");
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

			if (ImGui::SliderInt("FOV Changer", &newfov, -180, 180)) {
				Handler::writeProcessMemory<int>(newfov, (localPlayer + m_iDefaultFOV));
			}

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


}