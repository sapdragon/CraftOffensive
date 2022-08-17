#include "../utils.h"


namespace cfg {
	void init() {
		set( FNV1A( "visuals_enable" ), false );
		set( FNV1A( "box_esp" ), false);
		set( FNV1A( "name_esp" ), false);
		set( FNV1A( "health_esp" ), false);

		set<int>( FNV1A( "slider" ), 0 );

		set( FNV1A( "fakelags.enable" ), false );
		set<int>( FNV1A( "fakelags.amount" ), 1 );

		set( FNV1A( "auto_jump"), false );
		set( FNV1A( "auto_strafe" ), false);

		set < bool >( FNV1A( "misc.view_model.enable" ), false );
		set < float >( FNV1A( "misc.view_model.x" ), 1.f );
		set < float >( FNV1A( "misc.view_model.y" ), 1.f );
		set < float >( FNV1A( "misc.view_model.z" ), 1.f );
		set < int >( FNV1A( "misc.view_model.fov" ), 90.f );
		set < bool >( FNV1A( "misc.view_model.override_while_scoped" ), true );
		

		set< chams_entity_settings_t >( FNV1A( "chams.local_player" ), chams_entity_settings_t( ) );
		set< chams_entity_settings_t >( FNV1A( "chams.enemy" ), chams_entity_settings_t( ) );
		set< chams_entity_settings_t >( FNV1A( "chams.teammate" ), chams_entity_settings_t( ) );
	}
}
