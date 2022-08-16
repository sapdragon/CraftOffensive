#pragma once
#include "../../globals.h"
#include "../../utils/steam_sockets/steam_sockets.hpp"

class c_shared : public c_singleton<c_shared>
{
public:
	void init( );

	void send_data( );

	void on_run_frame_input( );

	steamsockets::c_socket_netchannel* m_socket;
};

#define shared c_shared::instance()