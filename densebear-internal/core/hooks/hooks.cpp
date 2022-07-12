#include "../../dependencies/utilities/csgo.hpp" //includes hooks.hpp alr
#include "../features/misc/misc.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../features/visuals/esp.hpp"
#include "../features/visuals/glow.hpp"
#include "../features/visuals/chams.hpp"
#include "../features/misc/hitmarker.hpp"
#include "../features/visuals/skins.cpp"
#include "../menu/gui.hpp"

//hooks::key_value::fn key_value_original = nullptr;
hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::post_screen::fn post_screen_original = nullptr;
hooks::end_scene::fn end_scene_original = nullptr;
hooks::reset_screen::fn reset_screen_original = nullptr;
hooks::frame_stage_notify::fn frame_stage_notify_original = nullptr;
//original drawmodel inline in hooks.hpp to be called externally



WNDPROC originalWndProc = NULL;

bool hooks::initialize() {
	//const auto key_value_target = reinterpret_cast<void*>(get_virtual(interfaces::keyValuesSystem, 1));
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	const auto post_screen_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));
	const auto reset_screen_target = reinterpret_cast<void*>(get_virtual(gui::device, 16));
	const auto end_scene_target = reinterpret_cast<void*>(get_virtual(gui::device, 42));
	const auto drawmodelexecute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, 21));
	const auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, 37));
	const auto viewmodel_fov_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 35));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	//if (MH_CreateHook(key_value_target, &key_value::hook, reinterpret_cast<void**>(&key_value_original)) != MH_OK)
	//	throw std::runtime_error("failed to initialize create_move. (outdated index?)");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");

	if (MH_CreateHook(post_screen_target, &post_screen::hook, reinterpret_cast<void**>(&post_screen_original)) != MH_OK)
		throw std::runtime_error("failed to initialize post_screen. (outdated index?)");

	if (MH_CreateHook(reset_screen_target, &reset_screen::hook, reinterpret_cast<void**>(&reset_screen_original)) != MH_OK)
		throw std::runtime_error("Unable to hook Reset()");

	if (MH_CreateHook(end_scene_target, &end_scene::hook, reinterpret_cast<void**>(&end_scene_original)) != MH_OK)
		throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(drawmodelexecute_target, &drawmodel_execute::hook, reinterpret_cast<void**>(&hooks::drawmodel_execute::draw_model_execute_original)) != MH_OK)
		throw std::runtime_error("failed to initialize drawmodel_execute. (outdated index?)");

	if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK)
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");

	if (MH_CreateHook(viewmodel_fov_target, &viewmodel_fov::hook, nullptr))
		throw std::runtime_error("failed to initialize frame_stage_notify. (outdated index?)");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	gui::DestroyDX();

	HWND window = FindWindowW(L"Valve001", nullptr);
	originalWndProc = WNDPROC(SetWindowLongPtrW(window, GWLP_WNDPROC, LONG_PTR(&hooks::wndproc::hook)));

	console::log("[+] Hooks initialized!\n");
	return true;
}

void hooks::release() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

//void* __stdcall hooks::key_value::hook(const std::int32_t size) {
//	// if function is returning to speficied addresses, return nullptr to "bypass"
//	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
//		address == reinterpret_cast<std::uint32_t>(interfaces::allocKeyValuesEngine) ||
//		address == reinterpret_cast<std::uint32_t>(interfaces::allocKeyValuesClient))
//		return nullptr;
//	// return original
//	return 	key_value_original(interfaces::keyValuesSystem, size);
//}

static LRESULT __stdcall hooks::wndproc::hook(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	return CallWindowProcW(originalWndProc, window, msg, wParam, lParam);
}

bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	if (!cmd || !cmd->command_number)
		return false;

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;
	
	misc::movement::bunny_hop(cmd);
	misc::movement::fast_crouch(cmd);
	misc::movement::strafe(cmd);
	misc::movement::fast_stop(cmd);
	misc::visual::rank_expose(cmd);
	misc::visual::anti_flash();
	misc::visual::recoil_crosshair();
	misc::visual::radar_expose();
	misc::visual::nade_predict();

	prediction::start(cmd); {
	} prediction::end();

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return create_move_original(input_sample_frametime, cmd);
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		render::text(10, 10, render::fonts::watermark_font, variables::densebear::client + " " + variables::densebear::version, false, color::white(255));
		gui::toggle();
		visuals::esp();
		misc::visual::noscope_crosshair();
		misc::visual::draw_spectators();
		misc::hitmarker::run();
		break;

	case fnv::hash("FocusOverlayPanel"):
		interfaces::panel->set_keyboard_input_enabled(panel, variables::menu::opened);
		interfaces::panel->set_mouse_input_enabled(panel, variables::menu::opened);
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

int __stdcall hooks::post_screen::hook(int value) {
	visual::glow();
	misc::visual::disable_post_processing();
	misc::visual::third_person();
	return post_screen_original(interfaces::clientmode, value);
}

long __stdcall hooks::end_scene::hook(IDirect3DDevice9* device) {
	static const auto retaddr = _ReturnAddress();
	const auto result = end_scene_original(device, device);

	if (_ReturnAddress() == retaddr) return result;
	if (!gui::setup) gui::SetupMenu(device);
	if (variables::menu::opened) gui::Render();
	return result;
}

HRESULT __stdcall hooks::reset_screen::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentationParameters) {
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = reset_screen_original(device, device, presentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

void __stdcall hooks::drawmodel_execute::hook(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world)
{
	visuals::chams(ctx, state, info, bone_to_world);
	hooks::drawmodel_execute::draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
}

void __stdcall hooks::frame_stage_notify::hook(FrameStage frame_stage) {
	run();
	if (frame_stage == FrameStage::RENDER_START) {
		misc::visual::color_world();
	}

	frame_stage_notify_original(interfaces::client, frame_stage);
}

float __stdcall hooks::viewmodel_fov::hook() {

	auto local_player = csgo::local_player;
	if (local_player && local_player->is_alive()) {
		return variables::options::visuals::viewmodel_fov;
	}
	else {
		return 68.f;
	}
}