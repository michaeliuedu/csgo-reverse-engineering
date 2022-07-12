#include "../visuals/esp.hpp"

bool get_playerbox(player_t* ent, box& in)
{
	if (ent == nullptr || ent->dormant()) return false;

	vec3_t flb, brt, blb, frt, frb, brb, blt, flt;

	const auto origin = ent->abs_origin();
	const auto min = ent->collideable()->mins() + origin;
	const auto max = ent->collideable()->maxs() + origin;

	vec3_t points[] = {
		vec3_t(min.x, min.y, min.z),
		vec3_t(min.x, max.y, min.z),
		vec3_t(max.x, max.y, min.z),
		vec3_t(max.x, min.y, min.z),
		vec3_t(max.x, max.y, max.z),
		vec3_t(min.x, max.y, max.z),
		vec3_t(min.x, min.y, max.z),
		vec3_t(max.x, min.y, max.z)
	};

	if (!interfaces::debug_overlay->world_to_screen(points[3], flb) || !interfaces::debug_overlay->world_to_screen(points[5], brt)
		|| !interfaces::debug_overlay->world_to_screen(points[0], blb) || !interfaces::debug_overlay->world_to_screen(points[4], frt)
		|| !interfaces::debug_overlay->world_to_screen(points[2], frb) || !interfaces::debug_overlay->world_to_screen(points[1], brb)
		|| !interfaces::debug_overlay->world_to_screen(points[6], blt) || !interfaces::debug_overlay->world_to_screen(points[7], flt))
		return false;

	vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	float left = flb.x;
	float top = flb.y;
	float right = flb.x;
	float bottom = flb.y;

	for (int i = 1; i < 8; ++i)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	try {
		in.x = static_cast<int>(left);
		in.y = static_cast<int>(top);
		in.w = static_cast<int>(right - left);
		in.h = static_cast<int>(bottom - top);
	}
	catch (...) { return false; };

	return true;
}

const color history{ 255, 255, 255,50 };
const color history_outline{ 0,0,0,60 };

void visuals::esp()
{
	if (!variables::options::visuals::esp)
		return;

	if (!interfaces::engine->is_in_game() || !csgo::local_player) return;

	//eneitylist error or client_class or somthing: interface sig/pattern interfaces::globals->max_clients;//err?
	for (size_t i = 1; i < 64; i++) {
		auto entity = static_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
		if (!entity || !csgo::local_player || entity == csgo::local_player || entity->health() <= 0 || entity->dormant()) continue;
		if (entity->team() == csgo::local_player->team()) continue;

		boxes(entity);
	}
}

void visuals::boxes(player_t* entity)
{
	box bbox;
	if (!get_playerbox(entity, bbox)) return;
	render::draw_rect(bbox.x, bbox.y, bbox.w, bbox.h, color::white(255));
	render::draw_rect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, entity->dormant() ? history_outline : color(0, 0, 0));
	render::draw_rect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, entity->dormant() ? history_outline : color(0, 0, 0));

	//HEALTH
	box temp(bbox.x - 5, bbox.y + (bbox.h - bbox.h * (std::clamp(entity->health(), 0, 100) / 100.f)), 1, bbox.h * (std::clamp(entity->health(), 0, 100) / 100.f) - (entity->health() >= 100 ? 0 : -1));
	box temp_bg(bbox.x - 5, bbox.y, 1, bbox.h);

	auto health_color = color((255 - entity->health() * 2.55), (entity->health() * 2.55), 0, 255);
	std::string healthStr = std::to_string(entity->health());

	if (entity->health() > 100)
		health_color = color(0, 255, 0);

	render::draw_filled_rect(temp_bg.x - 1, temp_bg.y - 1, temp_bg.w + 2, temp_bg.h + 2, color(0, 0, 0, 255));
	render::draw_filled_rect(temp.x, temp.y, temp.w, temp.h, color(health_color));

	//ARMOR
	box tempar(bbox.x, bbox.y + bbox.h + 5, bbox.w * (std::clamp(entity->armor(), 0, 100) / 100.f) - (entity->armor() >= 100 ? 0 : -1), 1);
	box temp_bgar(bbox.x, bbox.y + bbox.h + 5, bbox.w, 1);

	render::draw_filled_rect(temp_bgar.x - 1, temp_bgar.y - 1, temp_bgar.w + 2, temp_bgar.h + 2, color(1, 1, 1, 255));
	render::draw_filled_rect(tempar.x, tempar.y, tempar.w, tempar.h, color(87, 144, 250));

	//NAME
	player_info_t info;
	interfaces::engine->get_player_info(entity->index(), &info);
	auto red = 255;
	auto green = 255;
	auto blue = 255;

	std::string print(info.fakeplayer ? std::string("[BOT] ").append(info.name).c_str() : info.name);
	std::transform(print.begin(), print.end(), print.begin(), ::tolower);

	render::text(bbox.x + (bbox.w / 2), bbox.y - 13, render::fonts::watermark_font, print, true, color(red, green, blue, 255));

	//FLAGS
	std::string Money = std::to_string(entity->money());
	std::string HK = std::to_string(entity->has_heavy_armor());
	std::string Flashed = std::to_string(entity->is_flashed());
	std::string IsDefusing = std::to_string(entity->is_defusing());
	std::string IsScoped = std::to_string(entity->is_scoped());

	int MoneyH = render::get_text_size(render::fonts::watermark_font, "$" + Money).y;
	int HKH = render::get_text_size(render::fonts::watermark_font, HK).y;
	int FlashedH = render::get_text_size(render::fonts::watermark_font, Flashed).y;
	int DefusingH = render::get_text_size(render::fonts::watermark_font, IsDefusing).y;
	int ScopedH = render::get_text_size(render::fonts::watermark_font, IsScoped).y;
	if (entity->has_heavy_armor())
	{
		HK = "HK";
	}
	else {
		HK = "";
		HKH = 0;
	}
	if (entity->is_flashed())
	{
		Flashed = "Flashed";
	}
	else {
		Flashed = "";
		FlashedH = 0;
	}
	if (entity->is_defusing())
	{
		IsDefusing = "Defusing!";
	}
	else {
		IsDefusing = "";
		DefusingH = 0;
	}
	if (entity->is_scoped())
	{
		IsScoped = "Scoped";
	}
	else {
		IsScoped = "";
		ScopedH = 0;
	}
	MoneyH = render::get_text_size(render::fonts::watermark_font, "$" + Money).y;
	HKH = render::get_text_size(render::fonts::watermark_font, HK).y;
	FlashedH = render::get_text_size(render::fonts::watermark_font, Flashed).y;
	DefusingH = render::get_text_size(render::fonts::watermark_font, IsDefusing).y;
	ScopedH = render::get_text_size(render::fonts::watermark_font, IsScoped).y;

	if (const auto weapon = entity->active_weapon()) {
		if (const auto weaponInfo = weapon->get_weapon_data()); //err
		render::text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + 10, render::fonts::watermark_font, interfaces::localize->findutf8(weapon->get_weapon_data()->weapon_name), true, color(255, 0, 0));
	}

	render::text(bbox.x + bbox.w + 5, bbox.y + 2, render::fonts::watermark_font, "$" + Money, false, color(77, 219, 115)); //money
	render::text(bbox.x + bbox.w + 5, bbox.y + 2 + MoneyH + 2, render::fonts::watermark_font, HK, false, color(87, 144, 250)); //hk
	render::text(bbox.x + bbox.w + 5, bbox.y + 2 + MoneyH + 2 + HKH + 2, render::fonts::watermark_font, Flashed, false, color::white()); //flashed
	render::text(bbox.x + bbox.w + 5, bbox.y + 2 + MoneyH + 2 + HKH + 2 + FlashedH + 2, render::fonts::watermark_font, IsDefusing, false, color(250, 129, 125)); //is defusing
	render::text(bbox.x + bbox.w + 5, bbox.y + 2 + MoneyH + 2 + HKH + 2 + FlashedH + 2 + DefusingH + 2, render::fonts::watermark_font, IsScoped, false, color::white()); //is scoped

	auto pStudioModel = interfaces::model_info->get_studio_model(entity->model());
	if (!pStudioModel)
		return;

	static matrix_t pBoneToWorldOut[128];
	if (entity->setup_bones(pBoneToWorldOut, 128, 256, 0))
	{
		for (int i = 0; i < pStudioModel->bones_count; i++)
		{
			studio_bone_t* pBone = pStudioModel->bone(i);
			if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
				continue;

			vec2_t vBonePos1;
			if (!math::world_to_screen(vec3_t(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
				continue;

			vec2_t vBonePos2;
			if (!math::world_to_screen(vec3_t(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
				continue;
			if (entity->team() == csgo::local_player->team())
			{
				render::draw_line(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y, color::white());
			}
			else if (entity->team() != csgo::local_player->team())
			{
				render::draw_line(vBonePos1.x, vBonePos1.y, vBonePos2.x, vBonePos2.y, color::white());
			}
		}
	}
}

//const char* ConcatWeapon(const char* weapon_name) {
//	return ((std::string(weapon_name)).substr(7, (strlen)(weapon_name)-1)).c_str();
//}