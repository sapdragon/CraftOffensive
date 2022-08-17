#include "../utils.h"

namespace cfg_internal {
	template <class T>
	void jsonify( nlohmann::json& j, bool load, uint32_t key )
	{
		if ( j.is_null( ) && load )
			return;

		if ( !load )
			j = cfg::get<T>( key );
		else
			cfg::set<T>( key, j.get<T>( ) );
	}
	void jsonify_basic( nlohmann::json& j, bool load, bool& key )
	{
		if ( j.is_null( ) && load )
			return;

		if ( !load )
			j = key;
		else
			key = j.get<bool>();
	}
	void jsonify_basic( nlohmann::json& j, bool load, int& key )
	{
		if ( j.is_null( ) && load )
			return;

		if ( !load )
			j = key;
		else
			key = j.get<int>( );
	}
	void jsonify_basic( nlohmann::json& j, bool load, float& key )
	{
		if ( j.is_null( ) && load )
			return;

		if ( !load )
			j = key;
		else
			key = j.get<float>( );
	}
	void jsonify_basic( nlohmann::json& j, bool load, col_t& key )
	{
		if ( j.is_null( ) && load )
			return;

		if ( !load ) {
			j[ 0 ] = key.r( );
			j[ 1 ] = key.g( );
			j[ 2 ] = key.b( );
			j[ 3 ] = key.a( );
		}
		else
			key = col_t(j[0].get<int>( ), j[ 1 ].get<int>( ), j[ 2 ].get<int>( ), j[ 3 ].get<int>( ));
	}
}

namespace cfg {

	void init( ) {

		set < bool >( FNV1A( "legitbot.aimbot.enable" ), false );

		set < std::array <bool, 18 > >( FNV1A( "legitbot.aimbot.hitboxes" ), { true, true,  true, true, true, true,  true, true, true, true,  true, true, true, true,  true, true, true, true } );

		set < float >( FNV1A( "legitbot.aimbot.fov" ), 25.f );
		set < float >( FNV1A( "legitbot.aimbot.smooth" ), 15.f );

		set < bool >( FNV1A( "legitbot.aimbot.flash_check" ), true );
		set < bool >( FNV1A( "legitbot.aimbot.jump_check" ), true );
		set < bool >( FNV1A( "legitbot.aimbot.smoke_check" ), true );


		set<bool>( FNV1A( "visuals_enable" ), false );
		set<bool>( FNV1A( "box_esp" ), false );
		set<bool>( FNV1A( "name_esp" ), false );
		set<bool>( FNV1A( "health_esp" ), false );

		set<bool>( FNV1A( "fakelags.enable" ), false );
		set<int>( FNV1A( "fakelags.amount" ), 1 );

		set<bool>( FNV1A( "auto_jump" ), false );
		set<bool>( FNV1A( "auto_strafe" ), false );

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

	nlohmann::json jsonk;

	std::string json_action( bool load ) 
	{
		cfg_internal::jsonify<bool>( jsonk[ "misc.view_model.enable"  ], load, FNV1A( "misc.view_model.enable"  ) );
		cfg_internal::jsonify<bool>( jsonk[ "misc.view_model.override_while_scoped" ], load, FNV1A( "misc.view_model.override_while_scoped" ) );
		cfg_internal::jsonify<int>( jsonk[  "misc.view_model.fov" ], load, FNV1A(  "misc.view_model.fov" ) );
		cfg_internal::jsonify<int>( jsonk[ "misc.view_model.x" ], load, FNV1A( "misc.view_model.x" ) );
		cfg_internal::jsonify<int>( jsonk[ "misc.view_model.y" ], load, FNV1A( "misc.view_model.y" ) );
		cfg_internal::jsonify<int>( jsonk[ "misc.view_model.z"  ], load, FNV1A( "misc.view_model.z"  ) );

		cfg_internal::jsonify<bool>( jsonk[ "visuals_enable" ], load, FNV1A( "visuals_enable" ) );
		cfg_internal::jsonify<bool>( jsonk[ "box_esp" ], load, FNV1A( "box_esp" ) );
		cfg_internal::jsonify<bool>( jsonk[ "name_esp" ], load, FNV1A( "name_esp" ) );
		cfg_internal::jsonify<bool>( jsonk[ "health_esp" ], load, FNV1A( "health_esp" ) );
		cfg_internal::jsonify<bool>( jsonk[ "auto_jump" ], load, FNV1A( "auto_jump" ) );
		cfg_internal::jsonify<bool>( jsonk[ "auto_strafe" ], load, FNV1A( "auto_strafe" ) );

		cfg_internal::jsonify<bool>( jsonk[ "fakelags.enable" ], load, FNV1A( "fakelags.enable" ) );
		cfg_internal::jsonify<int>( jsonk[ "fakelags.amount" ], load, FNV1A( "fakelags.amount" ) );

		auto chams_action = [ ] ( uint32_t hash, std::string prefix, bool load ) {
			auto chams = get< chams_entity_settings_t >( hash );

			cfg_internal::jsonify_basic( jsonk[ "chams" ][ prefix ][ "invisible" ][ "enable" ], load, chams.m_invisible.m_enable );
			cfg_internal::jsonify_basic( jsonk[ "chams" ][ prefix ][ "visible" ][ "enable" ], load, chams.m_visible.m_enable );

			for ( auto a = 0; a < 6; a++ ) {
				cfg_internal::jsonify_basic( jsonk[ "chams" ][ prefix ][ "visible" ][ "mat" ][ a ][ "enable" ], load, chams.m_visible.m_materials[ a ].m_enable );
				cfg_internal::jsonify_basic( jsonk[ "chams" ][ prefix ][ "visible" ][ "mat" ][ a ][ "material" ], load, chams.m_visible.m_materials[ a ].m_material );
				cfg_internal::jsonify_basic( jsonk[ "chams" ][ prefix ][ "visible" ][ "mat" ][ a ][ "color" ], load, chams.m_visible.m_materials[ a ].m_color );
				cfg_internal::jsonify_basic( jsonk[ "chams" ][ prefix ][ "invisible" ][ "mat" ][ a ][ "enable" ], load, chams.m_invisible.m_materials[ a ].m_enable );
				cfg_internal::jsonify_basic( jsonk[ "chams" ][ prefix ][ "invisible" ][ "mat" ][ a ][ "material" ], load, chams.m_invisible.m_materials[ a ].m_material );
				cfg_internal::jsonify_basic( jsonk[ "chams" ][ prefix ][ "invisible" ][ "mat" ][ a ][ "color" ], load, chams.m_invisible.m_materials[ a ].m_color );
			}
		};

		chams_action( FNV1A( "chams.local_player" ), "local", load );
		chams_action( FNV1A( "chams.enemy" ), "enemy", load );
		chams_action( FNV1A( "chams.teammate" ), "teammate", load );

		return jsonk.dump( );
	}
}
