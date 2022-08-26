#pragma once
#include "json/json.hpp"

struct chams_material_settings_t
{
	bool m_enable = true;
	int m_material = 0;
	std::string label = "Unnamed";
	col_t m_color = col_t( 255, 0, 0 );
};

namespace cfg {
	extern void init( );
	extern std::string json_action( bool load );

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