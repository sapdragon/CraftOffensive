#include "aimbot.hpp"

void c_aimbot::on_create_move( )
{
	m_best_fov = 10000000;
	m_best_distance = 10000000;
	m_best_position = vec3_t( 0, 0, 0 );
	if ( !globals::m_local || !globals::m_local->is_alive( ) )
		return;

	if ( cfg::get( FNV1A( "legitbot.aimbot.flash_check" ) ) && globals::m_local->get_flash_alpha( ) > 0.f )
		return;

	if ( cfg::get( FNV1A( "legitbot.aimbot.jump_check" ) ) && !globals::m_local->get_flags().has( FL_ONGROUND )  )
		return;

	auto local_weapon = globals::m_local->get_active_weapon( );

	if ( !local_weapon || !local_weapon->is_gun( ) )
		return;

	auto local_eye_pos = globals::m_local->get_eye_pos( );

	if ( !local_eye_pos.is_valid( ) )
		return;

	qangle_t view_angle;

	interfaces::m_engine->get_view_angles( view_angle );

	float fov_settings =  cfg::get < float >( FNV1A( "legitbot.aimbot.fov" ) )  * 5.f;

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
			if ( !cfg::get <std::array<bool, 18> >( FNV1A( "legitbot.aimbot.hitboxes" ) ).at( hitbox_id ) )
				continue;

			auto hitbox = hitbox_set->get_hitbox( hitbox_id );

			if ( !hitbox )
				continue;

			auto hitbox_position = hitbox->get_position( bone_cache->m_cached_bones );

			if ( !hitbox_position.is_valid( ) )
				continue;

			vec3_t hitbox_on_screen = vec3_t( );

			if ( !render::world_to_screen(hitbox_position, hitbox_on_screen ) )
					continue;

			float distance = local_eye_pos.dist_to( hitbox_position );
	
			float fov = vec3_t( ImGui::GetIO( ).DisplaySize.x / 2, ImGui::GetIO( ).DisplaySize.y / 2, 0 ).dist_to( hitbox_on_screen );

			if ( fov <= fov_settings )
			{
				m_best_position = hitbox_position;
				m_best_distance = distance;

			}

		}
	}



	if ( m_best_position.empty( ) )
		return;

	auto aim_angle = math::calculate_angle( local_eye_pos, m_best_position );

	aim_angle.normalize( );

	auto delta = aim_angle - view_angle;

	auto final_angle = view_angle + ( delta / ( ( interfaces::m_global_vars->m_interval_per_tick * ( 1.0 / interfaces::m_global_vars->m_interval_per_tick )) * cfg::get < float >( FNV1A( "legitbot.aimbot.smooth" ) ) ) );


	interfaces::m_engine->set_view_angles( final_angle );
}