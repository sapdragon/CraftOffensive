#include "dormant.hpp"

void c_dormant_system::on_item_equip( i_game_event* event_info )
{
	/* getting player index from user id*/
	int player_index = interfaces::m_engine->get_player_for_user_id( event_info->get_int( _( "userid" ) ) );

	/* setup dormant data from event */
	m_dormant_players[ player_index ].m_last_weapon_id = event_info->get_int( _( "defindex" ) );
	m_dormant_players[ player_index ].m_last_weapon_type = event_info->get_int( _( "weptype" ) );
}

void c_dormant_system::on_radar_data_received( c_process_spotted_entity_update* message )
{
	if ( !globals::m_local )
		return;
	int entity_updates_size = *( int* ) ( ( DWORD ) ( message ) +0xC );
	if ( entity_updates_size <= 0 )
		return;

	for ( int i = 0; i < entity_updates_size; i++ )
	{
		DWORD entity = *( DWORD* ) ( *( DWORD* ) ( ( DWORD ) message + 0x8 ) + 4 * i );
		if ( !entity )
			continue;

		int entity_index = *( int* ) ( entity + 0x8 );
		if ( entity_index <= 0 || entity_index > 64 )
			continue;

		int class_id = *( int* ) ( entity + 0xC );
		if ( class_id != C_CS_PLAYER )
			continue;

		vec3_t origin = *( vec3_t* ) ( entity + 0x10 ) * 4;

		if ( origin.empty( ) )
			continue;

		m_dormant_players[ entity_index ].m_last_dormant_time = interfaces::m_global_vars->m_real_time;
		m_dormant_players[ entity_index ].m_origin = origin;
	}	


}

void c_dormant_system::on_shared_esp_received( uint32_t entity_index, vec3_t origin, float health )
{
	m_dormant_players[ entity_index ].m_last_dormant_time = interfaces::m_global_vars->m_real_time;
	m_dormant_players[ entity_index ].m_origin = origin;
	m_dormant_players[ entity_index ].m_last_dormant_health = health;
}

void c_dormant_system::reset_player( c_cs_player* player )
{
	int player_index = player->get_index( );

	m_dormant_players[ player_index ].m_origin = player->get_abs_origin( );
	m_dormant_players[ player_index ].m_network_origin = player->get_abs_origin( );

	m_dormant_players[ player_index ].m_last_non_dormant_time = interfaces::m_global_vars->m_real_time;
	m_dormant_players[ player_index ].m_last_dormant_time = interfaces::m_global_vars->m_real_time;
	m_dormant_players[ player_index ].m_last_weapon_id = 0;
	m_dormant_players[ player_index ].m_last_weapon_type = -1;
	m_dormant_players[ player_index ].m_last_dormant_health = player->get_health( );
}

