#pragma once
#include "../hooks.h"

void __cdecl hooks::engine_dll::host_run_frame_input::hook( void*, void*, float accumulated_extra_samples, bool final_tick )
{
	shared->on_run_frame_input( );
	return host_run_frame_input_original( accumulated_extra_samples, final_tick );
}

void __fastcall hooks::engine_dll::get_player_info::hook( void* ecx, void* edx, int index, player_info_t* info )
{
	get_player_info_original( ecx, edx, index, info );

/*	if ( !globals::m_local || !cfg::get( FNV1A( "misc.sharerd.name" ) ) )
		return;

	setup nickname 
	std::string new_name = _("");
	new_name += info->m_name;
	new_name += _( " | CRFT" );

	
	if ( shared->m_socket->is_connected( info->m_friends_id ) )
		strcpy( info->m_name, new_name.c_str( ) ); */
	
	//interfaces::m_cvar_system->console_print( col_t( 0, 255, 0 ), "Get Player info called | Name %s | User ID: %i \n", info->m_name, info->m_user_id );
}