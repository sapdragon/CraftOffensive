#include "../hooks.h"

void __stdcall hooks::client_mode::override_view::hook(view_setup_t* view) {

	if( globals::m_local )
		if( cfg::get( FNV1A( _("misc.view_model.override_while_scoped") ) ) || !globals::m_local->is_scoped() )
			view->m_fov = cfg::get < int >( FNV1A( _( "misc.view_model.fov" ) ) );

	misc->on_override_view( view );

	override_view_original(view);
}