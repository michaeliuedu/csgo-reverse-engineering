#include "glow.hpp"
#include <limits.h>

void visual::glow() {

	if (!variables::options::visuals::glow)
		return;

	if (!interfaces::engine->is_in_game() || !csgo::local_player) return;

	for (auto i = 0; i < interfaces::glow_manager->size; i++) {
		auto& glow = interfaces::glow_manager->objects[i];
		if (glow.unused() || !glow.pEntity) continue;
		const auto clientClass = static_cast<entity_t*>(glow.pEntity)->client_class();

		if (!clientClass) continue;

		switch (clientClass->class_id) {
		case ccsplayer:
		{
			const auto player = static_cast<player_t*>(glow.pEntity);
			if (!player->is_alive()) break;
			if (player->team() == csgo::local_player->team()) {
				if (variables::options::visuals::friendly)
					glow.set(variables::options::visuals::fg.color[0], variables::options::visuals::fg.color[1], variables::options::visuals::fg.color[2], variables::options::visuals::fg.color[3]);
			}
			else if (player->team() != csgo::local_player->team()) {
				if (variables::options::visuals::enemy) {
					glow.set(variables::options::visuals::eg.color[0], variables::options::visuals::eg.color[1], variables::options::visuals::eg.color[2], variables::options::visuals::eg.color[3]);
				}
			}
			break;
		}

		case cchicken:
		{
			if (variables::options::visuals::chicken)
				glow.set(variables::options::visuals::cg.color[0], variables::options::visuals::cg.color[1], variables::options::visuals::cg.color[2], variables::options::visuals::cg.color[3]);
			break;
		}

		case cplantedc4:
		{
			if (variables::options::visuals::bomb)
				glow.set(variables::options::visuals::bg.color[0], variables::options::visuals::bg.color[1], variables::options::visuals::bg.color[2], variables::options::visuals::bg.color[3]);
			break;
		}
		
		default:
		{
			if ((clientClass->class_id >= 232 && clientClass->class_id <= 273) || clientClass->class_id == cak47 || clientClass->class_id == cdeagle) { //revolver missing
				if (variables::options::visuals::weapon)
					glow.set(variables::options::visuals::wg.color[0], variables::options::visuals::wg.color[1], variables::options::visuals::wg.color[2], variables::options::visuals::wg.color[3]);
				break;
			}

		}

		}
	}
}