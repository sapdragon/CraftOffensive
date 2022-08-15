#include "../utils.h"

namespace cfg {
	void init() {
		set( FNV1A( "visuals_enable" ), false );
		set( FNV1A( "box_esp" ), false);
		set( FNV1A( "name_esp" ), false);
		set( FNV1A( "health_esp" ), false);

		set<int>( FNV1A( "slider" ), 0 );

		set( FNV1A( "auto_jump"), false );
		set( FNV1A( "auto_strafe" ), false);
	}
}
