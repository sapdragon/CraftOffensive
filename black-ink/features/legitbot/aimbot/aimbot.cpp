#include "aimbot.hpp"

void c_aimbot::on_create_move( )
{
	m_best_fov = 10000000;
	m_best_distance = 10000000;
	m_best_angle = qangle_t( 0, 0, 0 );
	if ( !globals::m_local || !globals::m_local->is_alive( ) )
		return;

	auto local_weapon = globals::m_local->get_active_weapon( );

	if ( !local_weapon || !local_weapon->is_gun( ) )
		return;

	auto local_eye_pos = globals::m_local->get_eye_pos( );

	if ( !local_eye_pos.is_valid( ) )
		return;

	qangle_t view_angle;

	interfaces::m_engine->get_view_angles( view_angle );


	for ( int i = 0; i < interfaces::m_global_vars->m_max_clients; i++ )
	{ 
		auto player = reinterpret_cast< c_cs_player* > ( interfaces::m_entity_list->get_client_entity( i ) );

		if ( !player || player == (c_cs_player*) globals::m_local ||  !player->is_player( ) || !player->is_alive( ) || player->is_dormant( ) || !player->is_enemy( globals::m_local ) )
			continue;

		auto hdr = interfaces::m_model_info->get_studio_model( player->get_model( ) );

		if ( !hdr )
			continue;

		mstudiohitboxset_t* hitbox_set = ( mstudiohitboxset_t* ) hdr->get_hitbox_set( player->get_hitbox_set( ) );

		if ( !hitbox_set )
			continue;

		auto bone_cache = player->get_bone_cache( );

		if ( !bone_cache )
			continue;

		if ( !bone_cache->m_cached_bones )
			continue;

		for ( int hitbox_id = 0; hitbox_id < HITBOX_LEFT_FOREARM; hitbox_id++ )
		{
			auto hitbox = hitbox_set->get_hitbox( hitbox_id );

			if ( !hitbox )
				continue;

			auto hitbox_position = hitbox->get_position( bone_cache->m_cached_bones );

			if ( !hitbox_position.is_valid( ) )
				continue;

			auto aimbot_angle = math::calculate_angle( local_eye_pos, hitbox_position );

			m_best_angle.normalize( );

			float fov = math::get_fov( view_angle, aimbot_angle );

			if ( fov * 5.f > cfg::get < float > ( FNV1A( "legitbot.aimbot.fov" ) ) )
				continue;

			float distance = local_eye_pos.dist_to( hitbox_position );

			if ( fov < m_best_fov || ( fov == m_best_fov && distance < m_best_distance  ) )
			{
				m_best_angle = aimbot_angle;
				m_best_fov = fov;
				m_best_distance = distance;

			}

		}
	}

	if ( m_best_angle.empty( ) )
		return;

	m_best_angle.normalize( );

	interfaces::m_engine->set_view_angles( m_best_angle );
}