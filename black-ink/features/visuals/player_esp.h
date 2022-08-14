#pragma once
#include "../../globals.h"

class c_player_esp : public c_singleton<c_player_esp> {
public:
	void on_paint();

	void draw_box(c_cs_player* player, vec3_t pos, vec3_t top, col_t color);
	void draw_name(c_cs_player* player, int index, vec3_t pos, vec3_t top, col_t color);
	void draw_health(c_cs_player* player, vec3_t pos, vec3_t top);
};

#define player_esp c_player_esp::instance()
