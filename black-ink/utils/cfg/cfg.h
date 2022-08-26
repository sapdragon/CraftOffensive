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

	float fov = 0;
	float smooth = 0;

	std::array<bool, 18> hitboxes = {
		false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false
	};

	bool flash_check = false;
	bool jump_check = false;
	bool smoke_check = false;

	struct {
		bool enable = false;

		int after = 0;

		float fov = 0;
		float smooth = 0;
		float pitch = 0;
		float yaw = 0;
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

	inline std::vector < chams_material_settings_t> local_player_visible;
	inline std::vector < chams_material_settings_t> enemy_visible;
	inline std::vector < chams_material_settings_t> teammates_visible;
	inline std::vector < chams_material_settings_t> local_player_invisible;
	inline std::vector < chams_material_settings_t> enemy_invisible;
	inline std::vector < chams_material_settings_t> teammates_invisible;

	template<class T = bool>
	T get( uint32_t hash ) {
		return std::any_cast<T>(m_items[hash]);
	}

	template<class T = bool>
	void set( uint32_t hash, T value ) {
		m_items[ hash ] = value;
	}
}