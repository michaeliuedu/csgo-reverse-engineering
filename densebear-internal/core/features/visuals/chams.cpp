#include "chams.hpp"

void __stdcall visuals::chams(i_mat_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {

	if (!variables::options::visuals::chams)
		return;

	if (info.model)
	{
		const char* modelName = interfaces::model_info->get_model_name(info.model);

		if (strstr(modelName, "models/player"))
		{
			entity_t* modelEnt = (entity_t*)interfaces::entity_list->get_client_entity(info.entity_index);

			if (!modelEnt || modelEnt == csgo::local_player || modelEnt->team() == csgo::local_player->team())
			{
				hooks::drawmodel_execute::draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);
				return;
			}

			i_material* material = interfaces::material_system->find_material("models/player/ct_fbi/ct_fbi_glass", nullptr, true, nullptr);

			float colorInvis[3] = { 255, 255, 0 };
			float colorvis[3] = { 0, 255, 255 };

			hooks::drawmodel_execute::draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);

			material->alpha_modulate(1.f);

			material->set_material_var_flag(material_var_ignorez, true);
			material->color_modulate(colorInvis[0], colorInvis[1], colorInvis[2]);
			interfaces::model_render->override_material(material);
			hooks::drawmodel_execute::draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);


			material->set_material_var_flag(material_var_ignorez, false);
			material->color_modulate(colorvis[0], colorvis[1], colorvis[2]);
			interfaces::model_render->override_material(material);
			hooks::drawmodel_execute::draw_model_execute_original(interfaces::model_render, ctx, state, info, bone_to_world);

		}
	}
}
