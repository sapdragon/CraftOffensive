#include "shared.hpp"
#include "proto/shared.pb.h"
#include "chams/shared_chams.hpp"
#include "esp/shared_esp.hpp"
void on_new_request_handler( SteamNetworkingMessagesSessionRequest_t* request )
{
	/* setup new connection*/
	steamsockets::client_connection connection;
	connection.m_steam_id = request->m_identityRemote.GetSteamID( ).GetAccountID( );
	connection.m_port = 58;

	shared->m_socket->add_connection( connection );
	shared->m_socket->add_opened_port( 58 );

	/*  accept connection */
	interfaces::m_steam_networking_messages->AcceptSessionWithUser( request->m_identityRemote );

#ifdef _DEBUG
	std::string reason = "[Socket ]  New session request  SteamID :" + std::to_string( request->m_identityRemote.GetSteamID( ).GetAccountID( ) ) + "\n";

	interfaces::m_cvar_system->console_print( col_t( 0, 255, 0 ), reason.c_str( ) );
#endif


	SharedHandshake handshake{};

	/* send hand shake*/
	shared->m_socket->send_message_to_connected_user( steamsockets::message_type_t::SHARED_INIT, &handshake, interfaces::m_steam_user->GetSteamID( ).GetAccountID( ) );
}

void on_new_request_failed( SteamNetworkingMessagesSessionFailed_t* failed_request )
{
#ifdef _DEBUG
	std::string reason = "[Socket ]  Session Request Failure Reason:" + std::to_string( failed_request->m_info.m_eEndReason ) + " SteamID : " + std::to_string( failed_request->m_info.m_identityRemote.GetSteamID( ).GetAccountID( ) ) + "\n";

	interfaces::m_cvar_system->console_print( col_t( 255, 0, 0 ), reason.c_str() );
#endif
}


void c_shared::init( )
{
	m_socket = new steamsockets::c_socket_netchannel( );

	/* add opened port*/
	m_socket->add_opened_port( 58 );

	/* add all handlers*/
	steamsockets::c_sockethandler* handler = new steamsockets::c_sockethandler;
	handler->add_handler( steamsockets::message_type_t::SHARED_ESP, &shared_esp::handler );
	handler->add_handler( steamsockets::message_type_t::SHARED_CHAMS, &shared_chams::handler );

	/* set handler to socket channel*/
	m_socket->set_handler( handler );

	/* set on new connection callback */
	interfaces::m_steam_networking_utils->SetGlobalCallback_MessagesSessionRequest( on_new_request_handler );
	interfaces::m_steam_networking_utils->SetGlobalCallback_MessagesSessionFailed( on_new_request_failed );

#ifdef _DEBUG
	interfaces::m_cvar_system->console_print( col_t( 0, 255, 0 ), "Shared setuped succefully!\n" );
#endif

}

void c_shared::send_data( )
{
	if ( !globals::m_local || !interfaces::m_engine->is_in_game( ) )
		return;

	/* send data to all */
	shared_esp::send_data( );
	//shared_chams::send_data( );
}

void c_shared::on_run_frame_input( )
{
	m_socket->new_frame( );
}
