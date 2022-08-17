#include "../hooks.h"

int __fastcall hooks::networking::send_datagram::hook( void* ecx, uint32_t edx, void* data )
{
	return hooks::send_datagram_original( ecx, edx, data );
}

void __fastcall hooks::networking::packet_end::hook( void* ecx, void* edx )
{
	int commands_acknowledged = interfaces::m_client_state->m_command_ack - interfaces::m_client_state->m_last_command_ack;

	if ( commands_acknowledged <= 0 )
		return;

	if ( interfaces::m_client_state->m_delta_tick == interfaces::m_client_state->m_clock_drift_mgr.m_server_tick )
	{
		dormant->on_post_network_data_received( );
	}

	return hooks::packet_end_original( ecx, edx);

}

void __cdecl hooks::networking::cl_move::hook( float frame_time, bool final_tick )
{
	fake_lags->set_maximium_chock( 14 );
	return hooks::cl_move_original( frame_time, final_tick );
}
