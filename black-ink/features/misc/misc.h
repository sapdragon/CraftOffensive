#pragma once
#include "../../globals.h"

class c_misc : public c_singleton<c_misc> {
private:
	void force_viewmodel_origin( );
public:
	void on_override_view( view_setup_t* setup );
};
#define misc c_misc::instance()