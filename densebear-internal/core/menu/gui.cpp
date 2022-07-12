#include "gui.hpp"
#include "../features/misc/misc.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void gui::debug_switch_b(std::string module, bool val) {
	std::string ED;
	if (val == true) ED = "Enabled";
	else ED = "Disabled";

	std::cout << module + ": [" + ED + "]\n";
}

void gui::debug_switch(std::string module, bool val, std::string config) {
	std::string ED; std::string str;
	if (val == true) ED = "Enabled";
	else ED = "Disabled";
	try {
		/*for (auto a : t)  str += std::to_string(a);*/
		std::cout << module + ": [" + ED + "] | Value: " << config << "\n";
	}
	catch (...) {
		std::cout << module + ": [" + ED + "]\n";
	}
}

void gui::toggle() {
	if (GetAsyncKeyState(VK_INSERT) & 1)
		variables::menu::opened = !variables::menu::opened;
}

bool gui::SetupWindowClass(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = DefWindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(NULL);
	windowClass.hIcon = NULL;
	windowClass.hCursor = NULL;
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowName;
	windowClass.hIconSm = NULL;

	if (!RegisterClassEx(&windowClass))return false;
	return true;
}

void gui::DestroyWindowClass() noexcept
{
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::SetupWindow(const char* windowName) noexcept
{
	window = CreateWindow(windowClass.lpszClassName, windowName, WS_OVERLAPPEDWINDOW,
		0, 0, 100, 100, 0, 0, windowClass.hInstance, 0);

	if (!window) return false;
	return true;
}

void gui::DestroyHWindow() noexcept
{
	if (window) DestroyWindow(window);
}

bool gui::SetupDX() noexcept
{
	const auto handle = GetModuleHandle("d3d9.dll");
	if (!handle) return false;

	using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);
	const auto create = reinterpret_cast<CreateFn>(GetProcAddress(handle, "Direct3DCreate9"));

	if (!create) return false;
	d3d9 = create(D3D_SDK_VERSION);

	if (!d3d9) return false;

	D3DPRESENT_PARAMETERS params = {};
	params.BackBufferWidth = 0;
	params.BackBufferHeight = 0;
	params.BackBufferFormat = D3DFMT_UNKNOWN;
	params.BackBufferCount = 0;
	params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	params.MultiSampleQuality = NULL;
	params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	params.hDeviceWindow = window;
	params.Windowed = 1;
	params.EnableAutoDepthStencil = 0;
	params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	params.Flags = NULL;
	params.FullScreen_RefreshRateInHz = 0;
	params.PresentationInterval = 0;

	if (d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_NULLREF,
		window,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&params,
		&device
	) < 0) return false;
	return true;
}

void gui::DestroyDX() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d9)
	{
		d3d9->Release();
		d3d9 = nullptr;
	}
}

void gui::initialize()
{
	if (!SetupWindowClass("hackClass001")) throw std::runtime_error("Failed to create window class");
	if (!SetupWindow("Densebear")) throw std::runtime_error("Failed to create window");
	if (!SetupDX()) throw std::runtime_error("Failed to create dxd9 device");

	DestroyHWindow();
	DestroyWindowClass();
}

void gui::theme() {
	ImGuiStyle* style = &ImGui::GetStyle();

	style->WindowRounding = 0.0f;
	style->ChildRounding = 0.0f;
	style->FrameRounding = 0.0f;
	style->GrabRounding = 0.0f;
	style->PopupRounding = 0.0f;
	style->ScrollbarRounding = 0.0f;

	style->WindowBorderSize = 0;
	style->WindowTitleAlign = ImVec2(0.5, 0.5);
	style->WindowMinSize = ImVec2(900, 430);

	style->FramePadding = ImVec2(8, 6);

	style->Colors[ImGuiCol_TitleBg] = ImColor(233, 75, 60, 255);
	style->Colors[ImGuiCol_TitleBgActive] = ImColor(233, 75, 60, 255);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImColor(0, 0, 0, 130);

	style->Colors[ImGuiCol_Button] = ImColor(31, 30, 31, 255);
	style->Colors[ImGuiCol_ButtonActive] = ImColor(41, 40, 41, 255);
	style->Colors[ImGuiCol_ButtonHovered] = ImColor(41, 40, 41, 255);

	style->Colors[ImGuiCol_Separator] = ImColor(70, 70, 70, 255);
	style->Colors[ImGuiCol_SeparatorActive] = ImColor(76, 76, 76, 255);
	style->Colors[ImGuiCol_SeparatorHovered] = ImColor(76, 76, 76, 255);

	style->Colors[ImGuiCol_FrameBg] = ImColor(37, 36, 37, 255);
	style->Colors[ImGuiCol_FrameBgActive] = ImColor(37, 36, 37, 255);
	style->Colors[ImGuiCol_FrameBgHovered] = ImColor(37, 36, 37, 255);

	style->Colors[ImGuiCol_Header] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderActive] = ImColor(0, 0, 0, 0);
	style->Colors[ImGuiCol_HeaderHovered] = ImColor(46, 46, 46, 255);
}

void gui::SetupMenu(LPDIRECT3DDEVICE9 device) noexcept
{
	auto params = D3DDEVICE_CREATION_PARAMETERS{};
	device->GetCreationParameters(&params);
	window = params.hFocusWindow;

	originalWindowProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess)));

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;

	ImFontConfig CustomFont;
	CustomFont.FontDataOwnedByAtlas = false;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 2.5;
	icons_config.OversampleV = 2.5;

	io.Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(CFont), sizeof(CFont), 15.f, &CustomFont);
	io.Fonts->AddFontFromMemoryCompressedTTF(fdat, fsize, 15.0f, &icons_config, icons_ranges);
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	setup = true;
}

void gui::release() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(originalWindowProc));
}

void gui::Graphics() noexcept {
	ImGui::Columns(2);
	ImGui::SetColumnOffset(1, 200);
	{
		ImGui::Spacing();
		if (ImGui::Button(ICON_FA_HOME  "  Home", ImVec2(200 - 15, 45))) Tab = 0;
		ImGui::Spacing();
		if (ImGui::Button(ICON_FA_EYE  "  Visuals", ImVec2(200 - 15, 45))) Tab = 1;
		ImGui::Spacing();
		if (ImGui::Button(ICON_FA_BOLT  "  Legit", ImVec2(200 - 15, 45))) Tab = 2;
		ImGui::Spacing();
		if (ImGui::Button(ICON_FA_ARROWS  "  Miscellaneous", ImVec2(200 - 15, 45))) Tab = 3;
		ImGui::Spacing();

		ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 60);
		if (ImGui::Button(ICON_FA_BOMB  "  Uninject", ImVec2(200 - 15, 45))) variables::densebear::terminate = true;

		ImGui::Spacing();
	}

	ImGui::NextColumn();
	{
		if (Tab == 0) {
			ImGui::Spacing();

			ImGui::BeginChild("C2", ImVec2(685, 410), false);

			ImGui::Spacing();

			ImGui::TextWrapped("Densebear private cheats is currently in alpha stage: v0.0.2. Some modules may be erroneous or have some discrepencies due to the infancy of this cheat."
				"This cheat was independently developed by @bear. Feel free to distribute and work improve upon modules to your liking. Do not distribute for any proprietary endeavors or claim original creation.");

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			ImGui::TextWrapped("Current features and descriptions: ");

			if (ImGui::CollapsingHeader("Visuals")) {
				ImGui::TextWrapped("Give yourself advantages that identify player, give information, or simply change models.");
				ImGui::Spacing();
				ImGui::BulletText("Glow - Basic ESP, rendering an outline even when player is occluded");
				ImGui::BulletText("ESP - Powerful information providing utility");
				ImGui::BulletText("Chams - Powerful internal glow, Customizable");
				ImGui::BulletText("Viewmodel FOV - Viewmodel field of view of gun and hand models");
				ImGui::BulletText("FOV - Alter the native camera field of view");

			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Legit")) {
				ImGui::TextWrapped("Toggle legit cheats for closet hacking. Useful for competitive but not rage or HvH.");
				ImGui::Spacing();

			}
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("Miscellaneous")) {
				ImGui::TextWrapped("Have a slight athletic advantage. Perfect mechanics that require years of practice.");
				ImGui::Spacing();
				ImGui::BulletText("Bunnyhop - Quick movement by bouncing around");
				ImGui::BulletText("Crouch Spam - Teabagging pretty much, no fatigue");
				ImGui::BulletText("Anti-Strafe - Strafe midair while jumping for better movement");
				ImGui::BulletText("Fast Stop - Aids counterstrafing by reducing velocity");

				ImGui::BulletText("Radar - Renders enemies on the radar, even when they are not visible");
				ImGui::BulletText("Reveal Ranks - Reveals enemy ranks");
				ImGui::BulletText("Noscope Crosshair - Gives a raw indicator to where the middle of your screen");
				ImGui::BulletText("Spectator List - Know who is watching to closet");
				ImGui::BulletText("Disable Post Processing - Make the game ugly?");
				ImGui::BulletText("Recoil Crosshair - Track bulltets better. Crosshair moves with recoil");
				ImGui::BulletText("Hitmarkers - Hit effects and hit sounds");
				ImGui::BulletText("World Color - Change the ambient color of models");
				ImGui::BulletText("Third Person - See your player model");
				ImGui::BulletText("Gernade Prediction - Draws predictions based off of gernade tossing");

			}

			ImGui::Spacing();
			ImGui::SetCursorPosY(ImGui::GetWindowHeight() - 45);
			ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 120);

			if (ImGui::Button(ICON_FA_CODE, ImVec2(50, 40))) {
				ShellExecute(0, 0, "https://github.com/bigbeawr/csgo-loader", 0, 0, SW_SHOW);
			}

			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_HEART, ImVec2(50, 40))) {
				ShellExecute(0, 0, "https://github.com/bigbeawr", 0, 0, SW_SHOW);
			}

			ImGui::EndChild();
		}

		if (Tab == 1) {
			ImGui::Spacing();
			ImGui::BeginChild("C2", ImVec2(685, 410), false);

			if (ImGui::CollapsingHeader("Engine Glow")) {	
				if (ImGui::Checkbox("Enabled", &variables::options::visuals::glow)) debug_switch_b("Engine Glow", variables::options::visuals::glow);
				ImGui::Spacing();

				ImGui::Checkbox("Enemy", &variables::options::visuals::enemy);
				ImGui::ColorEdit4("Enemy Color", variables::options::visuals::eg.color.data(), ImGuiColorEditFlags_AlphaBar);
				ImGui::Spacing();

				ImGui::Checkbox("Friendly", &variables::options::visuals::friendly);
				ImGui::ColorEdit4("Friendly Color", variables::options::visuals::fg.color.data(), ImGuiColorEditFlags_AlphaBar);
				ImGui::Spacing();

				ImGui::Checkbox("Chicken", &variables::options::visuals::chicken);
				ImGui::ColorEdit4("Chicken Color", variables::options::visuals::cg.color.data(), ImGuiColorEditFlags_AlphaBar);
				ImGui::Spacing();

				ImGui::Checkbox("Weapon", &variables::options::visuals::weapon);
				ImGui::ColorEdit4("Weapon Color", variables::options::visuals::wg.color.data(), ImGuiColorEditFlags_AlphaBar);
				ImGui::Spacing();

				ImGui::Checkbox("Planted Bomb", &variables::options::visuals::bomb);
				ImGui::ColorEdit4("Bomb Color", variables::options::visuals::bg.color.data(), ImGuiColorEditFlags_AlphaBar);
				ImGui::Spacing();

				if (ImGui::Checkbox("Chams", &variables::options::visuals::chams)) debug_switch_b("Chams", variables::options::visuals::chams);
				ImGui::Spacing();

			}				
			


			if (ImGui::CollapsingHeader("Advanced")) {
				if (ImGui::Checkbox("Utility ESP", &variables::options::visuals::esp)) debug_switch_b("Utility ESP", variables::options::visuals::esp);
				ImGui::Spacing();
			}

			if (ImGui::CollapsingHeader("Camera")) {
				if (ImGui::SliderFloat("Viewmodel FOV", &variables::options::visuals::viewmodel_fov, -50.f, 160.f)) debug_switch_b("Viewmodel FOV", variables::options::visuals::viewmodel_fov);
				ImGui::Spacing();

				if (ImGui::SliderFloat("FOV", &variables::options::visuals::fov, -179.9f, 179.9f)) {
					csgo::local_player->default_fov() = variables::options::visuals::fov;
					debug_switch_b("FOV", variables::options::visuals::fov);
				}
				ImGui::Spacing();
			}

			ImGui::EndChild();
		}
		if (Tab == 2) {
			ImGui::Spacing();

			if (ImGui::CollapsingHeader("General")) {
				ImGui::Spacing();
			}

			ImGui::Spacing();
		}

		if (Tab == 3) {
			ImGui::Spacing();
			ImGui::BeginChild("C3", ImVec2(685, 410), false);

			if (ImGui::CollapsingHeader("Movement")) {
				if (ImGui::Checkbox("Bunnybop", &variables::options::misc::movement::bhop)) debug_switch_b("Bunnybop", variables::options::misc::movement::bhop);
				ImGui::Spacing();

				if (ImGui::Checkbox("Crouch Spam", &variables::options::misc::movement::crouchspam)) debug_switch_b("Crouch Spam", variables::options::misc::movement::crouchspam);
				ImGui::Spacing();

				if (ImGui::Checkbox("Anti-Strafe", &variables::options::misc::movement::airstrafe)) debug_switch_b("Anti-Strafe", variables::options::misc::movement::airstrafe);
				ImGui::Spacing();

				if (ImGui::Checkbox("Fast Stop", &variables::options::misc::movement::quickstop)) debug_switch_b("Fast Stop", variables::options::misc::movement::quickstop);
				ImGui::Spacing();
			}

			if (ImGui::CollapsingHeader("Player")) {
				if (ImGui::Checkbox("Radar", &variables::options::misc::visuals::radarexpose))debug_switch_b("Radar", variables::options::misc::visuals::radarexpose);
				ImGui::Spacing();

				if (ImGui::Checkbox("Antiflash", &variables::options::misc::visuals::antiflash))debug_switch_b("Antiflash", variables::options::misc::visuals::antiflash);
				ImGui::Spacing();

				if (ImGui::Checkbox("Reveal Ranks", &variables::options::misc::visuals::rankexpose))debug_switch_b("Rank Expose", variables::options::misc::visuals::rankexpose);
				ImGui::Spacing();

				if (ImGui::Checkbox("Noscope Crosshair", &variables::options::misc::visuals::noscope_crosshair))debug_switch_b("Noscope Crosshair", variables::options::misc::visuals::noscope_crosshair);
				ImGui::Spacing();

				if (ImGui::Checkbox("Spectator List", &variables::options::misc::visuals::spectator_list))debug_switch_b("Spectator List", variables::options::misc::visuals::spectator_list);
				ImGui::Spacing();

				if (ImGui::Checkbox("Disable Post-Processing", &variables::options::misc::visuals::disable_pp))debug_switch_b("Disable Post-Processing", variables::options::misc::visuals::disable_pp);
				ImGui::Spacing();

				if (ImGui::Checkbox("Recoil Crosshair", &variables::options::misc::visuals::recoil_crosshair))debug_switch_b("Recoil Crosshair", variables::options::misc::visuals::recoil_crosshair);
				ImGui::Spacing();

				if (ImGui::Checkbox("Hitmarkers", &variables::options::misc::visuals::hitmarkers))debug_switch_b("Hitmarkers", variables::options::misc::visuals::hitmarkers);
				ImGui::Spacing();

				if (ImGui::Checkbox("Third Person", &variables::options::misc::visuals::thirdperson)) debug_switch_b("Third Person", variables::options::misc::visuals::thirdperson);
				ImGui::SliderFloat("Camera Offset", &variables::options::misc::visuals::tp, 50.f, 200.f);
				ImGui::Spacing();

				if (ImGui::Checkbox("World Color", &variables::options::misc::visuals::worldcolor)) debug_switch_b("World Color", variables::options::misc::visuals::worldcolor);
				ImGui::Spacing();

				ImGui::ColorEdit4("Prop Color", variables::options::misc::visuals::pc.color.data(), ImGuiColorEditFlags_AlphaBar);
				ImGui::ColorEdit3("Sky Color", variables::options::misc::visuals::wc.color.data());

				if (ImGui::Checkbox("Gernade Prediction", &variables::options::misc::visuals::nadepredict)) debug_switch_b("Gernade Prediction", variables::options::misc::visuals::nadepredict);
				ImGui::Spacing();

			}

			ImGui::Spacing();
			ImGui::EndChild();
		}
	}
}

void gui::Render() noexcept
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	gui::theme();
	{
		ImGui::Begin("Densebear", &variables::menu::opened, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse); {
			gui::Graphics();
		}
		ImGui::End();
	}
	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (GetAsyncKeyState(VK_INSERT) & 1)variables::menu::opened = !variables::menu::opened;
	if (variables::menu::opened && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) return 1L;
	return CallWindowProc(gui::originalWindowProc, hWnd, msg, wParam, lParam);
}