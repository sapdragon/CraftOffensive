#include "shared.hpp"
#include "proto/shared.pb.h"
#include "esp/shared_esp.hpp"
void on_new_request_handler( SteamNetworkingMessagesSessionRequest_t* request )
{
	/* setup new connection*/
	steamsockets::client_connection connection;
	connection.m_steam_id = request->m_identityRemote.GetSteamID( ).GetAccountID( );
	connection.m_port = 58;

	shared->m_socket->add_connection( connection );
	shared->m_socket->add_opened_port( connection.m_port );

	/*  accept connection */
	interfaces::m_steam_networking_messages->AcceptSessionWithUser( request->m_identityRemote );


	SharedHandshake handshake{};

	/* send hand shake*/
	shared->m_socket->send_message_to_user( steamsockets::message_type_t::SHARED_INIT, &handshake, interfaces::m_steam_user->GetSteamID( ).GetAccountID( ) );
}

void c_shared::init( )
{
	m_socket = new steamsockets::c_socket_netchannel( );

	/* add opened port*/
	m_socket->add_opened_port( 58 );

	/* add all handlers*/
	steamsockets::c_sockethandler* handler = new steamsockets::c_sockethandler;
	handler->add_handler( steamsockets::message_type_t::SHARED_ESP, &shared_esp::handler );
	
	/* set handler to socket channel*/
	m_socket->set_handler( handler );

	/* set on new connection callback */
	interfaces::m_steam_networking_utils->SetGlobalCallback_MessagesSessionRequest( on_new_request_handler );

}

void c_shared::send_data( )
{
	/* send data to all */
	shared_esp::SendData( );

}

void c_shared::on_create_move( )
{
	m_socket->new_frame( );
}
