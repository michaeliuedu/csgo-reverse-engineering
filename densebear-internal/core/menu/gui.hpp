#pragma once
#include <d3d9.h>
#include "../../dependencies/utilities/csgo.hpp"
#include "framework.hpp"
#include "variables.hpp"

#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_dx9.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"

#include <stdexcept>

namespace gui {
	inline int Tab{ 0 };

	void debug_switch(std::string module, bool val, std::string config);
	void debug_switch_b(std::string module, bool val);

	void toggle();

	inline bool setup = false;
	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = {};
	inline WNDPROC originalWindowProc = nullptr;

	inline PDIRECT3D9 d3d9 = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;

	bool SetupWindowClass(const char* windowName) noexcept;
	void DestroyWindowClass() noexcept;

	bool SetupWindow(const char* windowName) noexcept;
	void DestroyHWindow() noexcept;

	bool SetupDX() noexcept;
	void DestroyDX() noexcept;

	void initialize();
	void theme();

	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void release() noexcept;
	void Render() noexcept;
	void Graphics() noexcept;
}