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

		set< chams_entity_settings_t >( FNV1A( "chams.local_player" ), chams_entity_settings_t( ) );
		set< chams_entity_settings_t >( FNV1A( "chams.enemy" ), chams_entity_settings_t( ) );
		set< chams_entity_settings_t >( FNV1A( "chams.teammate" ), chams_entity_settings_t( ) );
	}
}
