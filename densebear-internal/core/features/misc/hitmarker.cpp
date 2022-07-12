#include "hitmarker.hpp"

int hitmarker_time = 0;
bool death = false; 

void misc::hitmarker::run() {
	if (!interfaces::engine->is_connected() && !interfaces::engine->is_in_game())
		return;

	misc::hitmarker::draw();
}

void  misc::hitmarker::event(i_game_event* event) {
	if (!event || !variables::options::misc::visuals::hitmarkers)
		return;
	auto victim = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("userid")));
	if ((static_cast<player_t*>(victim)->health() - event->get_int("dmg_health") <= 0)) death = true;

	if (!csgo::local_player)
		return;

	auto attacker = interfaces::entity_list->get_client_entity(interfaces::engine->get_player_for_user_id(event->get_int("attacker")));
	if (!attacker)
		return;

	if (attacker == csgo::local_player) {
		hitmarker_time = 255;
		if(!death) interfaces::surface->play_sound("buttons\\arena_switch_press_02.wav");
		else interfaces::surface->play_sound("survival\\money_collect_01.wav");
	}
}


void  misc::hitmarker::draw() {

	interfaces::engine->get_screen_size(csgo::unupdated_screen_dim.x, csgo::unupdated_screen_dim.y);
	int width_mid = csgo::unupdated_screen_dim.x / 2;
	int height_mid = csgo::unupdated_screen_dim.y / 2;


	if (hitmarker_time > 0) {
		float alpha = hitmarker_time;
		color hm = color(255, 255, 255, alpha);
		color dm = color(255, 0, 0, alpha);

		int offset = (death ? 8 : 6);
		int endset = (death ? 4 : 3);

		render::draw_line(width_mid + offset, height_mid + offset, width_mid + endset, height_mid + endset, death ? dm : hm);
		render::draw_line(width_mid - offset, height_mid + offset, width_mid - endset, height_mid + endset, death ? dm : hm);
		render::draw_line(width_mid + offset, height_mid - offset, width_mid + endset, height_mid - endset, death ? dm : hm);
		render::draw_line(width_mid - offset, height_mid - offset, width_mid - endset, height_mid - endset, death ? dm : hm);

		hitmarker_time -= 2;
	}else if (death) death = false;

}

