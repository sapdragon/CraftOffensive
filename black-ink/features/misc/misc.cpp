#include "misc.h"

void c_misc::force_viewmodel_origin( )
{
	if ( !globals::m_local || !globals::m_local->is_alive( ) || interfaces::m_client_state->m_signon_state < 6 )
		return;

	/* Get view model */
	c_base_view_model* view_model =  (c_base_view_model* ) globals::m_local->get_view_model( ).get( ) ;
	if ( !view_model )
		return;

	vec3_t forward, right, up;
	math::angle_vectors( view_model->get_abs_angles( ), &forward, &right, &up );

	/* force modificated origin*/
	view_model->set_abs_origin( view_model->get_abs_origin( ) + ( right * cfg::get < float >( FNV1A(  "misc.view_model.x"  ) ) ) + ( forward * cfg::get < float >( FNV1A(  "misc.view_model.y"  ) ) ) + ( up * cfg::get < float >( FNV1A(  "misc.view_model.z"  ) ) ) );

}

void c_misc::on_override_view( view_setup_t* setup )
{
	if( cfg::get < bool >( FNV1A(  "misc.view_model.enable" ) ) )
		force_viewmodel_origin( );
}
