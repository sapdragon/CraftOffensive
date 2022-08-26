#include "aimbot.hpp"

bool CanSeePlayer( c_base_player* player, const vec3_t& pos )
{
	c_game_trace tr;
	ray_t ray( globals::m_local->get_eye_pos( ), pos );
	c_trace_filter filter;
	filter.m_skip = globals::m_local;

	interfaces::m_trace_system->trace_ray( ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr );

	return tr.m_hit_entity == player || tr.m_fraction > 0.97f;
}

void c_aimbot::setup_config( c_base_combat_weapon* weapon )
{
	if ( weapon->is_pistol( ) ) {
		current_settings = cfg::aimbot_pistol;
	}
	else if ( weapon->is_smg( ) ) {
		current_settings = cfg::aimbot_smg;
	}
	else if ( weapon->is_rifle( ) ) {
		current_settings = cfg::aimbot_rifle;
	}
	else if ( weapon->is_sniper( ) ) {
		current_settings = cfg::aimbot_sniper;
	}
	else if ( weapon->is_shotgun( ) ) {
		current_settings = cfg::aimbot_shotgun;
	}
	else if ( weapon->is_heavy( ) ) {
		current_settings = cfg::aimbot_heavy;
	}
}

void c_aimbot::render_fov( ) {
	render::circle( vec2_t(ImGui::GetIO().DisplaySize.x/2, ImGui::GetIO( ).DisplaySize.y / 2 ), current_settings.fov * 5.f, col_t(255, 0, 0));
}

void c_aimbot::on_create_move( )
{
	m_best_fov = 10000000;
	m_best_distance = 10000000;
	m_best_position = vec3_t( 0, 0, 0 );

	if ( !globals::m_local || !globals::m_local->is_alive( ) )
		return;

	render_fov( );

	if ( !(globals::m_cur_cmd->m_buttons & IN_ATTACK) )
		return;

	auto local_weapon = globals::m_local->get_active_weapon( );

	if ( !local_weapon || !local_weapon->is_gun( ) )
		return;

	setup_config( local_weapon );

	if ( !current_settings.enable )
		return;

	if ( current_settings.flash_check && globals::m_local->get_flash_alpha( ) > 0.f )
		return;

	if ( current_settings.jump_check && !globals::m_local->get_flags( ).has( FL_ONGROUND ) )
		return;

	auto local_eye_pos = globals::m_local->get_eye_pos( );

	if ( !local_eye_pos.is_valid( ) )
		return;

	qangle_t view_angle;

	interfaces::m_engine->get_view_angles( view_angle );

	float aimbot_fov_settings = current_settings.fov * 5.f;
	float rcs_fov_settings = current_settings.rcs.fov * 5.f;

	for ( int i = 0; i < interfaces::m_global_vars->m_max_clients; i++ )
	{
		auto player = reinterpret_cast< c_cs_player* > ( interfaces::m_entity_list->get_client_entity( i ) );

		if ( !player || player == ( c_cs_player* ) globals::m_local || !player->is_player( ) || !player->is_alive( ) || player->is_dormant( ) || !player->is_enemy( globals::m_local ) )
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
			if ( !current_settings.hitboxes.at( hitbox_id ) )
				continue;

			auto hitbox = hitbox_set->get_hitbox( hitbox_id );

			if ( !hitbox )
				continue;

			auto hitbox_position = hitbox->get_position( bone_cache->m_cached_bones );

			if ( !hitbox_position.is_valid( ) )
				continue;

			if ( !CanSeePlayer( player, hitbox_position ) )
				continue;

			vec3_t hitbox_on_screen = vec3_t( );

			if ( !render::world_to_screen( hitbox_position, hitbox_on_screen ) )
				continue;

			float distance = local_eye_pos.dist_to( hitbox_position );

			float fov = vec3_t( ImGui::GetIO( ).DisplaySize.x / 2, ImGui::GetIO( ).DisplaySize.y / 2, 0 ).dist_to( hitbox_on_screen );

			if ( fov > aimbot_fov_settings && fov > rcs_fov_settings )
				continue;

			if ( fov < m_best_fov )
			{
				m_best_position = hitbox_position;
				m_best_distance = distance;
				m_best_fov = fov;
			}
		}
	}

	if ( m_best_position.empty( ) )
		return;

	auto aim_angle = math::calculate_angle( local_eye_pos, m_best_position );

	aim_angle.normalize( );

	auto punch_angle = qangle_t( 0, 0, 0 );

	if ( m_best_fov <= rcs_fov_settings && globals::m_local->get_shots_fired( ) >= current_settings.rcs.after )
	{
		static auto recoil_scale = interfaces::m_cvar_system->find_var( FNV1A( "weapon_recoil_scale" ) )->get_float( );

		punch_angle = globals::m_local->get_aim_punch_angle( ) * recoil_scale;

		punch_angle.x *= current_settings.rcs.pitch;
		punch_angle.y *= current_settings.rcs.yaw;
	}

	auto delta = aim_angle - ( view_angle + punch_angle );
	auto final_angle = view_angle + ( delta / ( ( interfaces::m_global_vars->m_interval_per_tick * ( 1.0 / interfaces::m_global_vars->m_interval_per_tick ) ) * current_settings.smooth ));

	m_old_punch = punch_angle;

	if ( !current_settings.silent )
		interfaces::m_engine->set_view_angles( final_angle );
	else
		globals::m_cur_cmd->m_view_angles = final_angle;
}