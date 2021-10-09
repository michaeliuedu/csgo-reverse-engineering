#include "functions.h"
#include <iostream>
#include "offsets.h"
namespace bear {

	void Functions::gui_callback()
	{
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Densebear Private Cheats v0.0.1 alpha");
		ImGui::Text("Welcome to Densebear's private undetected cheat currently in alpha");

	
	}

}

