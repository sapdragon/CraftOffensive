#pragma once
#include <unordered_map>
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
		int16_t  m_message_type;
	};

	struct client_connection {
		int m_port{ 0 };
		int m_player_index{ 0 };
		uint32_t m_steam_id{ 0 };
		char m_name[ 33 ];
		char m_encryption_key[ 256 ];
	};

	typedef bool( __stdcall* handler_fn )( size_t size, const char* message );

	class sockethandler
	{
	public: 
		void process_message( int size, int16_t message_type, const char* message );
		void add_handler( int handler_type, handler_fn function )
		{
			m_handlers.at( handler_type ) = function;
		}
	private:
		std::unordered_map < int, handler_fn > m_handlers;
	};

	class socket_netchannel
	{
		void new_frame( );
		void add_connection( client_connection  connection )
		{
			m_connections.push_back( connection );
		}
		void set_handler( sockethandler* handler ) { 
			m_handler = handler;
		}

		void send_message_to_user( int16_t message_type, google::protobuf::Message* message, uint32_t steam_id, int port = 58);

		void SendMessageToAll( int16_t message_type, google::protobuf::Message* message, int port = 58 );

	private:
		sockethandler* m_handler;
		std::vector < int > m_open_ports;
		std::vector< client_connection > m_connections;
	};
}