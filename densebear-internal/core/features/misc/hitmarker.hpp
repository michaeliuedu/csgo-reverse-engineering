#pragma once
#include "../../../dependencies/utilities/csgo.hpp"
#include "../../menu/variables.hpp"

namespace misc {
	namespace hitmarker {
		void run();
		void event(i_game_event* event);
		void draw();
	}
}