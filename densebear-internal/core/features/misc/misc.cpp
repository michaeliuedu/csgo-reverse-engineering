#include "misc.hpp"

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::options::misc::movement::bhop)
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
};

void misc::movement::fast_crouch(c_usercmd* cmd) {
	if (!variables::options::misc::movement::crouchspam) 
		return;
	cmd->buttons |= in_bullrush;
}


void misc::movement::strafe(c_usercmd* user_cmd) {

	if (!variables::options::misc::movement::airstrafe)
		return;

	auto local_player = csgo::local_player;

	if (!local_player || !local_player->is_alive())
		return;

	if (local_player->flags() & fl_onground && !(user_cmd->buttons & in_jump))
		return;

	if (local_player->move_type() & (movetype_ladder | movetype_noclip))
		return;

	vec3_t viewangles;
	interfaces::engine->get_view_angles(viewangles);

	static bool side_switch = false;
	side_switch = !side_switch;

	user_cmd->forwardmove = 0.f;
	user_cmd->sidemove = side_switch ? 450.f : -450.f;

	float velocity_yaw = local_player->velocity().y;
	
	float rotation = std::clamp< float >((float)((std::atan2(15.f, local_player->velocity().length2d())) * (float)(180.f / math::pi)), 0.f, 90.f);

	float abs_yaw = std::fabs(math::normalize_angle(velocity_yaw - viewangles.y));

	float ideal_yaw_rotation = (side_switch ? rotation : -rotation) + (abs_yaw < 5.f ? velocity_yaw : viewangles.y);

	float cos_rot = std::cos(math::deg_to_rad(viewangles.y - ideal_yaw_rotation));
	float sin_rot = std::sin(math::deg_to_rad(viewangles.y - ideal_yaw_rotation));

	user_cmd->forwardmove = (cos_rot * user_cmd->forwardmove) - (sin_rot * user_cmd->sidemove);
	user_cmd->sidemove = (sin_rot * user_cmd->forwardmove) + (cos_rot * user_cmd->sidemove);
}

void misc::movement::fast_stop(c_usercmd* cmd)
{
	if (!variables::options::misc::movement::quickstop)
		return;

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (csgo::local_player->move_type() == movetype_noclip || csgo::local_player->move_type() == movetype_ladder || csgo::local_player->flags() & fl_onground || cmd->buttons & in_jump);
		return;

	if (cmd->buttons & (in_moveleft | in_moveright | in_forward | in_back))
		return;

	const auto velocity = csgo::local_player->velocity();
	const auto speed = velocity.length2d();
	if (speed < 15.0f)
		return;

	auto direction = velocity.toAngle();
	direction.y = cmd->viewangles.y - direction.y;

	const auto negatedDirection = vec3_t::fromAngle(direction) * -speed;
	cmd->forwardmove = negatedDirection.x;
	cmd->sidemove = negatedDirection.y;
}

void misc::visual::anti_flash() {
	if (!variables::options::misc::visuals::antiflash)
		return;

	if (!interfaces::engine->is_in_game() || !csgo::local_player) return;
	csgo::local_player->flash_duration() = 0;

}

void misc::visual::radar_expose() {
	if (!variables::options::misc::visuals::radarexpose)
		return;

	if (!interfaces::engine->is_in_game() || !csgo::local_player) return;
	for (int i = 1; i < 64; i++) {
		auto currententity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (currententity) currententity->spotted() = true;
	}

}

void misc::visual::rank_expose(c_usercmd* cmd) {

	if (!variables::options::misc::visuals::rankexpose)
		return;

	if (!interfaces::engine->is_in_game() || !csgo::local_player) return;

	if (cmd->buttons & in_score)
	interfaces::client->dispatch_user_message(cs_um_serverrankrevealall, 0, 0, nullptr);

}

void misc::visual::noscope_crosshair()
{
	if (!variables::options::misc::visuals::noscope_crosshair) return;

	if (!interfaces::engine->is_in_game() || !csgo::local_player || !csgo::local_player->is_alive()) return;;
	const auto weapon = csgo::local_player->active_weapon();
	if (!weapon) return;

	const auto data = interfaces::weapon_system->get_weapon_data(weapon->item_definition_index());
	if (!data) return;

	if (data->weapon_type != WEAPONTYPE_SNIPER_RIFLE) return;
	if (csgo::local_player->is_scoped()) return;

	interfaces::surface->get_screen_size(csgo::unupdated_screen_dim.x, csgo::unupdated_screen_dim.y);
	int x = csgo::unupdated_screen_dim.x / 2;
	int y = csgo::unupdated_screen_dim.y / 2;
	interfaces::surface->set_drawing_color(255, 0, 0);

	size_t crosshair_size = 3; //Ghetto rendering but works... sorta
	for (size_t i = 0; i < crosshair_size; i++) {
		render::draw_circle(x, y, i, 360, color::red());
	}
}

void misc::visual::draw_spectators() {

	if (!variables::options::misc::visuals::spectator_list)
		return;

	if (!interfaces::engine->is_in_game() || !csgo::local_player || !csgo::local_player->is_alive())
		return;;

	int spec_count = 0;

	for (int i = 1; i <= 64; i++) {
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || entity->is_alive() || entity == csgo::local_player)
			continue;


		const auto spectator = interfaces::entity_list->get_client_entity_handle(entity->observer_target());
		if (!spectator)
			continue;

		player_info_t player_info_spec{};
		if (!interfaces::engine->get_player_info(i, &player_info_spec)) continue;

		if (!strcmp(player_info_spec.name, "GOTV"))
			continue;

		if (spectator != csgo::local_player)
			continue;

		interfaces::surface->get_screen_size(csgo::unupdated_screen_dim.x, csgo::unupdated_screen_dim.y);
		render::text(csgo::unupdated_screen_dim.x - 90, csgo::unupdated_screen_dim.y/2 + 10 * spec_count, render::fonts::watermark_font, (player_info_spec.name), true, color::red());
		
		spec_count++;

	}
	render::text(csgo::unupdated_screen_dim.x - 50, csgo::unupdated_screen_dim.y / 2 - 20, render::fonts::watermark_font, "Spectators: " + spec_count, false, color::white());

}

void misc::visual::disable_post_processing() {
	static auto mat_postprocess_enable = interfaces::console->get_convar("mat_postprocess_enable");
	mat_postprocess_enable->set_value(variables::options::misc::visuals::disable_pp ? 0 : 1);
}

void misc::visual::recoil_crosshair() {
	static auto cl_crosshair_recoil = interfaces::console->get_convar("cl_crosshair_recoil");
	cl_crosshair_recoil->set_value(variables::options::misc::visuals::recoil_crosshair ? 1 : 0);
}

void misc::visual::color_world()
{

	interfaces::material_system->first_material();
	for (short h = interfaces::material_system->first_material(); h != interfaces::material_system->invalid_material_handle(); h = interfaces::material_system->next_material(h)) {
		const auto mat = interfaces::material_system->get_material(h);
		
		const std::string_view textureGroup = mat->get_texture_group_name();

		if (!mat || !mat->is_precached())
			continue;
		if ((textureGroup.starts_with("World") || textureGroup.starts_with("StaticProp"))) {
			if (variables::options::misc::visuals::worldcolor) {
				mat->alpha_modulate(variables::options::misc::visuals::pc.color[3]);
				mat->color_modulate(variables::options::misc::visuals::pc.color[0], variables::options::misc::visuals::pc.color[1], variables::options::misc::visuals::pc.color[2]);
			}
			else {
				mat->alpha_modulate(1.f);
				mat->color_modulate(1, 1, 1);
			}

		}
		else if (textureGroup.starts_with("SkyBox")) {
			if (variables::options::misc::visuals::worldcolor)
				mat->color_modulate(variables::options::misc::visuals::wc.color[0], variables::options::misc::visuals::wc.color[1], variables::options::misc::visuals::wc.color[2]);
			else {
				mat->alpha_modulate(1.f);
				mat->color_modulate(1, 1, 1);
			}
		}
		//For some reason, color modulate 1, 1, 1 is close to the default and we cant undo soooo...
	}
}

void misc::visual::third_person()
{
	interfaces::input->isCameraInThirdPerson = variables::options::misc::visuals::thirdperson;
	interfaces::input->cameraOffset.z = static_cast<float>(variables::options::misc::visuals::tp);
}

void misc::visual::nade_predict()
{
	static auto nade = interfaces::console->get_convar("cl_grenadepreview");
	nade->callbacks.size = 0;
	nade->set_value(variables::options::misc::visuals::nadepredict);

}