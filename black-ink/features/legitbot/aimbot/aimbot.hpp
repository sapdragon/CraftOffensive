#pragma once
#include "../../globals.h"

class c_aimbot {
private:
	float m_best_fov = 0.f;
	float m_best_distance = 0.f;
	vec3_t m_best_position = vec3_t( );
	qangle_t m_old_punch = qangle_t( );

private:
	aimbot_group_settings current_settings;

public:
	void setup_config( c_base_combat_weapon* weapon );
	void on_create_move( );
	void render_fov( );
};

inline c_aimbot* aimbot = new c_aimbot( );