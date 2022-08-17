#pragma once
#include "../../globals.h"

class c_fake_lags : public c_singleton<c_fake_lags> {
private:
	int m_choked_commands = 0;
	int m_maximim_choked = 0;

	bool reset_required = false;

	vec3_t m_networked_origin = vec3_t( );
public:
	void on_create_move( );

	void reset_data( );

	void set_maximium_chock( float maximum_chock ) { m_maximim_choked = maximum_chock; };
};
#define fake_lags c_fake_lags::instance()