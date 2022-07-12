#pragma once
#include "../../../dependencies/utilities/csgo.hpp"
#include "../../menu/variables.hpp"

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
		void fast_crouch(c_usercmd* cmd);
		void strafe(c_usercmd* cmd);
		void fast_stop(c_usercmd* cmd);

	};

	namespace visual {
		void anti_flash();
		void radar_expose();
		void rank_expose(c_usercmd* cmd);
		void noscope_crosshair();
		void draw_spectators();
		void disable_post_processing();
		void recoil_crosshair();
		void color_world();
		void third_person();
		void nade_predict();
	}
}
