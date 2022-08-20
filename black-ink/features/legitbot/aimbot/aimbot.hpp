#pragma once
#include "../../globals.h"

class c_aimbot : public c_singleton<c_aimbot> {
private:
	float m_best_fov = 0.f;
	float m_best_distance = 0.f;
	vec3_t m_best_position = vec3_t( );

public:

	void on_create_move( );
};
#define aimbot c_aimbot::instance()