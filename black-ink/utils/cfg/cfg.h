#pragma once
#include "json/json.hpp"

struct chams_material_settings_t
{
	bool m_enable = true;
	int m_material = 0;
	std::string label = "Unnamed";
	col_t m_color = col_t( 255, 0, 0 );
};

struct aimbot_group_settings {
	bool enable = false;
	bool silent = false;

	float fov = 15;
	float smooth = 25;

	std::array<bool, 18> hitboxes = {
		true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false
	};

	bool flash_check = false;
	bool jump_check = false;
	bool smoke_check = false;

	struct {
		bool enable = false;

		int after = 1;

		float fov = 1;
		float smooth = 1;
		float pitch = 1;
		float yaw = 1;
	} rcs;

	aimbot_group_settings( ) = default;
};

namespace cfg {
	extern void init( );
	extern std::string json_action( bool load );

	inline aimbot_group_settings aimbot_pistol = aimbot_group_settings();
	inline aimbot_group_settings aimbot_smg = aimbot_group_settings( );
	inline aimbot_group_settings aimbot_rifle = aimbot_group_settings( );
	inline aimbot_group_settings aimbot_shotgun = aimbot_group_settings( );
	inline aimbot_group_settings aimbot_heavy = aimbot_group_settings( );
	inline aimbot_group_settings aimbot_sniper = aimbot_group_settings( );

	inline nlohmann::json jsonk;
	inline std::unordered_map<uint32_t, std::any> m_items;

	inline chams_material_settings_t enemy_visible;
	inline chams_material_settings_t teammates_visible;
	inline chams_material_settings_t enemy_invisible;
	inline chams_material_settings_t teammates_invisible;

	template<class T = bool>
	T get( uint32_t hash ) {
		if ( m_items[ hash ].has_value( ) )
			return std::any_cast< T >( m_items[ hash ] );
		else
			return T( );
	}

	template<class T = bool>
	void set( uint32_t hash, T value ) {
		m_items[ hash ] = value;
	}
}