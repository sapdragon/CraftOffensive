#include "fakelags.h"

void c_fake_lags::on_create_move( )
{

	if ( !globals::m_local || !globals::m_local->is_alive( ) || globals::m_local->get_flags().has(FL_FROZEN ) )
		return;

	bool in_attack = false;

	auto* weapon = globals::m_local->get_active_weapon( );
	if ( weapon )
	{
		if ( weapon->get_cs_weapon_data( )->m_weapon_type == WEAPON_TYPE_GRENADE )
		{
			if ( weapon->get_throw_time( ) > 0.0f && !weapon->get_pin_pulled( ) )
				in_attack = true;
		}
		else
		{
			if ( !weapon || ( weapon->get_item_definition_index( ) != WEAPON_ZEUS_X27 && !weapon->is_gun( ) ) )
				return;

			float player_time = TICKS_TO_TIME( globals::m_local->get_tick_base( ) );

			if ( weapon->get_ammo( ) <= 0 || globals::m_local->get_next_attack( ) >= player_time )
				return;

			if ( globals::m_local->is_defusing ( )  )
				return;

			if ( globals::m_local->is_wait_for_no_attack( ) )
				return;

			if ( globals::m_local->player_state( ) )
				return;

			if ( weapon->get_item_definition_index( ) == WEAPON_ZEUS_X27 )
					if ( weapon->get_post_pone_fire_ready_time( ) >= player_time || weapon->get_activity() != 208 )
						return;
			

			

			if ( weapon->get_next_primary_attack( ) <= player_time )//g_EnginePrediction->CanAttack( true ) )
				if ( ( globals::m_cur_cmd->m_buttons & IN_ATTACK ) || ( ( globals::m_cur_cmd->m_buttons.has ( IN_ATTACK2)  ) && weapon->get_cs_weapon_data( )->m_weapon_type == WEAPON_TYPE_KNIFE ) )
					in_attack = true;
		}
	}
		if ( !cfg::get< bool >( FNV1A( "fakelags.enable" ) ) )
			return;

		m_choked_commands = interfaces::m_client_state->m_choked_commands;

		if ( reset_required )
		{
			if ( m_choked_commands < 14 )
			{
				globals::m_packet = false;
				return;
			}

			reset_required = false;
		}

		if ( m_choked_commands > m_maximim_choked || m_choked_commands >= cfg::get< int >( FNV1A( "fakelags.amount" ) ) ||
			( globals::m_local->get_origin( ) - m_networked_origin ).length_2d_sqr( ) > 4096.0f ||
			globals::m_local->get_flags( ).has( FL_FROZEN )
			||
			in_attack ) 
		{
			globals::m_packet = true;
			m_networked_origin = globals::m_local->get_origin( );
		}
		else
		{
			globals::m_packet = false;
		}
}

void c_fake_lags::reset_data( )
{
	m_networked_origin = vec3_t( );
	m_maximim_choked = 0;
	m_choked_commands = 0;
	reset_required = false;
}
