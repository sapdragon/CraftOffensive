#include "../features.h"

void c_player_esp::on_paint() {
	for (auto i = 1; i < interfaces::m_global_vars->m_max_clients; i++) {
		const auto player = static_cast<c_cs_player*>(interfaces::m_entity_list->get_client_entity(i));

		if (!player || !player->is_alive())
			continue;

		if (!globals::m_local)
			continue;

		bool is_team = player->get_team() == globals::m_local->get_team();

		if (player->get_origin() == vec3_t(0, 0, 0))
			continue;

		vec3_t min, max, pos, pos_3d, top, top_3d;
		player->get_render_bounds(min, max);

		pos_3d = player->get_abs_origin() - vec3_t(0, 0, 10);
		top_3d = pos_3d + vec3_t(0, 0, max.z + 10);

		if (render::world_to_screen(pos_3d, pos) && render::world_to_screen(top_3d, top)) {
			if (!is_team) {
				if (cfg::get<bool>(FNV1A("visuals_enable"))) {
					if (cfg::get<bool>(FNV1A("box_esp"))) {
						draw_box(player, pos, top, col_t(255, 255, 255, 255));
					}

					if (cfg::get<bool>(FNV1A("name_esp"))) {
						draw_name(player, i, pos, top, col_t(255, 255, 255, 255));
					}

					if (cfg::get<bool>(FNV1A("health_esp"))) {
						draw_health(player, pos, top);
					}
				}
			}
		}
	}
}

void c_player_esp::draw_box(c_cs_player* player, vec3_t pos, vec3_t top, col_t color) {
	float alpha = 255.f;

	int height = (pos.y - top.y);
	int width = height / 2;

	render::rect_angle(pos.x - width / 2, top.y, (pos.x - width / 2) + width, top.y + height, color, 1, 0);
	render::rect_angle((pos.x - width / 2) + 1, top.y + 1, (pos.x - width / 2) + width - 1, top.y + height - 1, col_t(20, 20, 20, alpha - 130), 1, 0);
	render::rect_angle((pos.x - width / 2) - 1, top.y - 1, (pos.x - width / 2) + width + 1, top.y + height + 1, col_t(20, 20, 20, alpha - 130), 1, 0);
}

void c_player_esp::draw_name(c_cs_player* player, int index, vec3_t pos, vec3_t top, col_t color) {
	player_info_t player_info;

	interfaces::m_engine->get_player_info(index, &player_info);

	auto size = fonts::m_tahoma14->CalcTextSizeA(12.f, FLT_MAX, 0.0f, player_info.m_name);
	int height = (pos.y - top.y);
	int weight = height / 2;

	render::text_string(pos.x - size.x / 2, top.y - 15, color, e_font_flags::FONT_DROP_SHADOW, fonts::m_tahoma14, player_info.m_name);
}

void c_player_esp::draw_health(c_cs_player* player, vec3_t pos, vec3_t top) {

	int player_hp = player->get_health();

	int hp_red = 255 - (player_hp * 2.55);

	int height = (pos.y - top.y);
	int width = height / 2;

	float offset = (height / 4.f) + 5;
	int hp = height - (UINT)((height * player_hp) / 100);
	static int old_hp = height - (UINT)((height * player_hp) / 100);

	if (hp < old_hp)
		old_hp++;

	auto size = fonts::m_tahoma14->CalcTextSizeA(12.f, FLT_MAX, 0.0f, std::to_string(player_hp).c_str());

	render::rect_filled_int((pos.x - width / 2) - 6, top.y, (pos.x - width / 2) - 1, top.y + height, col_t(20, 20, 20, 100));
	render::rect_filled_int((pos.x - width / 2) - 6 + 1, top.y + hp + 1, (pos.x - width / 2) - 2, top.y + height - 1, col_t(30, 255, 30, 255));

	if (player_hp < 90)
		render::text_string((pos.x - width / 2) - size.x + 5, top.y + hp - 4, col_t(255, 255, 255, 255), e_font_flags::FONT_OUTLINE, fonts::m_tahoma14, std::to_string(player_hp).c_str());
}