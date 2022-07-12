#pragma once

enum class FrameStage {
	UNDEFINED = -1,
	START,
	NET_UPDATE_START,
	NET_UPDATE_POSTDATAUPDATE_START,
	NET_UPDATE_POSTDATAUPDATE_END,
	NET_UPDATE_END,
	RENDER_START,
	RENDER_END
};

namespace hooks {


	bool initialize();
	void release();

	inline unsigned int get_virtual(void* _class, unsigned int index) { return static_cast<unsigned int>((*static_cast<int**>(_class))[index]); }

	//namespace key_value {
	//	using fn = void* (__thiscall*)(void*, const std::int32_t);
	//	void* __stdcall hook(const std::int32_t size);
	//};

	namespace create_move {
		using fn = bool(__stdcall*)(float, c_usercmd*);
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd);
	};

	namespace paint_traverse {
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force);
	}

	namespace post_screen {
		using fn = int(__thiscall*)(void*, int);
		int __stdcall hook(int value);
	}

	namespace end_scene {
		using fn = long(__thiscall*)(void*, IDirect3DDevice9*);
		long __stdcall hook(IDirect3DDevice9* device);
	}

	namespace reset_screen {
		using fn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		HRESULT __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentationParameters);
	}

	namespace drawmodel_execute {
		using fn = void(__thiscall*)(iv_model_render*, i_mat_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
		static void __stdcall hook(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world);
		inline fn draw_model_execute_original = nullptr;

	}

	namespace frame_stage_notify {
		using fn = void(__thiscall*)(i_base_client_dll*, FrameStage);
		void __stdcall hook(FrameStage frame_stage);
	}

	namespace viewmodel_fov {
		float __stdcall hook();
	}

	namespace wndproc {
		static LRESULT __stdcall hook(HWND window, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	}
}