#include "steam_sockets.hpp"
#include "../../sdk/interfaces.h"

void steamsockets::sockethandler::process_message( int size, int16_t message_type, const char* message )
{
}

void steamsockets::socket_netchannel::new_frame( )
{
	SteamNetworkingMessage_t* message{ nullptr };
	SteamNetworkingMessage_t* message_array{ nullptr };

	for ( auto port : m_open_ports )
	{
		int amount_recived_messages = interfaces::m_steam_networking_messages->ReceiveMessagesOnChannel( port, &message_array, 1 );

		for ( int i = 0; i < amount_recived_messages; i++ )
		{
			message = &( message_array[ i ] );

			if ( !message )
				continue;

			size_t message_size = message->GetSize( );

			if ( message_size < sizeof( message_header_t ) )
			{
				message->Release( );
				continue;
			}


			message_header_t* header = reinterpret_cast< message_header_t* >( ( void* ) message->GetData( ) );

			if ( header->m_message_size > ( message_size - sizeof( message_header_t ) ) )
			{
				message->Release( );
				continue;
			}

			if ( !strstr( header->m_header, _( "BIDEN" ) ) )
			{
				message->Release( );
				continue;
			}

			char* message_buffer = ( char* ) malloc( message_size );

			std::memcpy( message_buffer, message->GetData( ), message_size );

			header = reinterpret_cast< message_header_t* >( message_buffer );

			switch ( header->m_message_type )
			{
			case SHARED_SKINS:
			case SHARED_CHAMS:
			case SHARED_ESP:
				m_handler->process_message( header->m_message_size, header->m_message_type, ( const char* ) ( message_buffer + sizeof( message_header_t ) ) );

			case SHARED_INIT:
				client_connection new_connection{};
				new_connection.m_steam_id = message->m_identityPeer.GetSteamID( ).GetAccountID( );
				new_connection.m_port = 58;
				add_connection( new_connection );
				break;
			}
			free( message_buffer );
			message->Release( );

		}
	}
}

void steamsockets::socket_netchannel::send_message_to_user( int16_t message_type, google::protobuf::Message* message, uint32_t steam_id, int port )
{
	size_t data_size = message->ByteSize( );
	size_t message_size = sizeof( message_header_t ) + data_size;
	char* message_buffer = ( char* ) malloc( message_size );

	message->SerializePartialToArray( ( void* ) ( message_buffer + sizeof( message_size ) ), data_size );

	message_header_t* header = reinterpret_cast< message_header_t* >( message_buffer );

	strcpy( header->m_header, _( "BIDEN" ) );
	header->m_message_size = data_size;
	header->m_message_type = message_type;
	header->m_steam_id = interfaces::m_steam_user->GetSteamID( ).GetAccountID( );

	SteamNetworkingIdentity iden;

	CSteamID full_steam_id( steam_id, k_EUniversePublic, k_EAccountTypeIndividual );

	iden.SetSteamID64( full_steam_id.ConvertToUint64( ) );

	interfaces::m_steam_networking_messages->SendMessageToUser( iden, message_buffer, message_size, k_nSteamNetworkingSend_ReliableNoNagle | k_nSteamNetworkingSend_AutoRestartBrokenSession, port );
}

void steamsockets::socket_netchannel::SendMessageToAll( int16_t message_type, google::protobuf::Message* message, int port )
{
	for ( auto i = 1; i < interfaces::m_global_vars->m_max_clients; i++ ) {
		const auto player = static_cast< c_cs_player* >( interfaces::m_entity_list->get_client_entity( i ) );

		if ( !player->is_player( ) )
			continue;

		player_info_t info;

		interfaces::m_engine->get_player_info( i, &info );

		uint32_t steam_id = info.m_friends_id;

		send_message_to_user( message_type, message, port, steam_id);
	}
}