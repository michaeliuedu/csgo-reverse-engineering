#pragma once
#include <string>

struct color_4 {
	std::array<float, 4> color{ 1.0f, 1.0f, 1.0f, 1.0f };
};

struct color_3 {
	std::array<float, 3> color{ 1.0f, 1.0f, 1.0f };
};


namespace variables {
	namespace densebear {
		inline std::string client = "Densebear";
		inline std::string version = "v0.0.1";
		inline const char* console = "development";
		inline bool terminate = false;
	}

	namespace options {
		namespace misc {
			namespace movement {
				inline bool bhop = false;
				inline bool crouchspam = false;
				inline bool airstrafe = false;
				inline bool quickstop = false;
			}

			namespace visuals {
				inline bool antiflash = false;
				inline bool radarexpose = false;
				inline bool rankexpose = false;
				inline bool noscope_crosshair = false;
				inline bool spectator_list = false;
				inline bool disable_pp = false;
				inline bool recoil_crosshair = false;
				inline bool hitmarkers = false;
				inline bool thirdperson = false;
				inline float tp = 100.f;

				inline bool worldcolor = false;
				inline color_3 wc = { 1, 1, 1 };
				inline color_4 pc = { 1, 1, 1, 1 };

				inline bool nadepredict = false;
			}
		}
		namespace visuals {
			inline bool esp = false;
			inline bool glow = false;

			inline color_4 eg = { 255, 0, 0, 1 };
			inline bool enemy = false;
			inline color_4 fg = { 255, 0, 0, 1 };
			inline bool friendly = false;
			inline color_4 cg = { 255, 0, 0, 1 };
			inline bool chicken = false;
			inline color_4 wg = { 255, 0, 0, 1 };
			inline bool weapon = false;
			inline color_4 bg = { 255, 0, 0, 1 };
			inline bool bomb = false;

			inline bool chams = false;
			inline float viewmodel_fov = 68.f;
			inline float fov = 90.f;

		}
	}

	namespace menu {
		inline bool opened = true;
	}
}