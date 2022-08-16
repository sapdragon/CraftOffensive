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


void c_dormant_system::on_post_network_data_received( )
{
	m_sound_data.m_current_sound_data.clear( );

	interfaces::m_sound_engine->get_active_sounds( m_sound_data.m_current_sound_data );

	if ( !m_sound_data.m_current_sound_data.size( ) )
		return;
	c_game_trace trace;
	for ( int i = 0; i < m_sound_data.m_current_sound_data.size( ); i++ )
	{
		auto& sound = m_sound_data.m_current_sound_data[ i ];
		if ( sound.m_sound_source < 1 || sound.m_sound_source > interfaces::m_global_vars->m_max_clients )
			continue;

		if ( sound.m_origin->empty( ) || !is_valid_sound_data( sound ) )
			continue;

		auto player = reinterpret_cast < c_cs_player* > ( interfaces::m_entity_list->get_client_entity( sound.m_sound_source ) );

		if ( !player || !player->is_alive( ) || !player->is_dormant( ) )
			continue;

		vec3_t start = *sound.m_origin + vec3_t( 0.0f, 0.0f, 1.0f );
		vec3_t end = start - vec3_t( 0.0f, 0.0f, 100.0f );

		static auto simple_trace_filter_addr = utils::find_pattern_from_module( GetModuleHandleA( _( "client.dll" ) ), _( "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) ) + 0x3D;

		std::uintptr_t filter[ 4 ] = {
	*reinterpret_cast< std::uintptr_t* >( simple_trace_filter_addr ),
	reinterpret_cast< std::uintptr_t >( player ),
	( uintptr_t ) ( COLLISION_GROUP_NONE ),
	0
		};

		auto ray = ray_t(start, end );

		interfaces::m_trace_system->trace_ray( ray, MASK_PLAYERSOLID, ( c_trace_filter* ) ( filter ), &trace );


		if ( trace.m_all_solid )
			continue;

		// set dormant data
		m_dormant_players[ sound.m_sound_source ].m_origin = trace.m_fraction <= 0.97f ? trace.m_end_pos : *sound.m_origin;
		m_dormant_players[ sound.m_sound_source ].m_last_dormant_time = interfaces::m_global_vars->m_real_time;

		// spot player
		player->is_spotted( ) = true;

		/* m_bSpottedBy[LocalPlayerIdx] = true */
		*( bool* ) ( ( DWORD ) ( player ) +0x93E + interfaces::m_engine->get_local_player( )  ) = true;

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

bool c_dormant_system::is_valid_sound_data( sound_info_t sound )
{
	for ( int index = 0; index < m_sound_data.m_cached_sound_data.size( ); index++ )
		if ( sound.m_guid == m_sound_data.m_cached_sound_data[ index ].m_guid )
			return false;

	return true;
}

