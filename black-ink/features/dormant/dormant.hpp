#pragma once
#include "../../globals.h"

struct dormant_player_t
{
	float m_alpha = 0.0f;
	float m_last_dormant_time = 0.0f;
	float m_last_non_dormant_time = 0.0f;
		
	vec3_t m_origin = vec3_t( 0, 0, 0 );
	vec3_t m_network_origin = vec3_t( 0, 0, 0 );

	int m_last_dormant_health = 0;
	int m_last_weapon_id = 0;
	int m_last_weapon_type = 0;
};

class c_dormant_system : public c_singleton<c_dormant_system> {
public:
	void on_item_equip( i_game_event* event_info );
	void on_radar_data_received( c_process_spotted_entity_update* message );

	void on_shared_esp_received(uint32_t entity_index, vec3_t origin, float health );

	void reset_player( c_cs_player* player );
	void reset_data( ) { m_dormant_players.fill( dormant_player_t( ) ); }


private:
	std::array < dormant_player_t, 64 > m_dormant_players;

};
#define dormant c_dormant_system::instance()