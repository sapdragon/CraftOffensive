#pragma once
#include <map>
#include <google/protobuf/message.h>
namespace steamsockets
{
	enum message_type_t
	{
		SHARED_INIT,
		SHARED_ESP,
		SHARED_CHAMS,
		SHARED_SKINS
	};

	struct message_header_t
	{
		char m_header[ 6 ];
		uint32_t m_message_size;
		uint32_t m_steam_id;
		uint16_t  m_message_type;
	};

	struct client_connection {
		int m_port{ 0 };
		int m_player_index{ 0 };
		uint32_t m_steam_id{ 0 };
		char m_name[ 33 ];
		char m_encryption_key[ 256 ];
	};

	typedef bool( __stdcall* handler_fn )( size_t size, const char* message );

	class c_sockethandler
	{
	public: 
		void process_message( int size, uint16_t message_type, const char* message );
		void add_handler( int handler_type, handler_fn function )
		{
			//m_handlers.emplace( handler_type, function );
			m_handlers[handler_type] = function;
		}
	private:
		std::map < int, handler_fn > m_handlers;
	};

	class c_socket_netchannel
	{
	public:
		void new_frame( );
		void add_connection( client_connection  connection )
		{
			m_connections.push_back( connection );
		}
		void set_handler( c_sockethandler* handler ) {
			m_handler = handler;
		}

		void add_opened_port( int port ) { m_open_ports.push_back( port ); }


		void send_message_to_user( uint16_t message_type, google::protobuf::Message* message, int port, uint32_t steam_id );
		void send_message_to_connected_user( uint16_t message_type, ::google::protobuf::Message* message, uint32_t steam_id ) {
			for ( client_connection& connection : m_connections ) {
				if ( connection.m_steam_id == steam_id ) {
						send_message_to_user( message_type, message, connection.m_port, steam_id );
					break;
				}
			}
		}

		void SendMessageToAll( uint16_t message_type, google::protobuf::Message* message, int port = 58 );

	private:
		c_sockethandler* m_handler;
		std::vector < int > m_open_ports;
		std::vector< client_connection > m_connections;
	};
}