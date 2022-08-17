#pragma once
#include "json/json.hpp"
struct chams_material_settings_t
{
	bool m_enable = true;
	int m_material = 0;
	col_t m_color = col_t( 255, 0, 0 );
};

struct chams_settings_t
{
	bool m_enable = true;
	std::array < chams_material_settings_t, 6 > m_materials = { chams_material_settings_t (), chams_material_settings_t( ), chams_material_settings_t( ), chams_material_settings_t( ), chams_material_settings_t( ), chams_material_settings_t( ) };
};

struct chams_entity_settings_t
{
	chams_settings_t m_visible = chams_settings_t( );
	chams_settings_t m_invisible = chams_settings_t( );
};

namespace cfg {
	extern void init( );
	extern std::string json_action( bool load );

	inline nlohmann::json jsonk;
	inline std::unordered_map<uint32_t, std::any> m_items;

	template<class T = bool>
	T get( uint32_t hash ) {
		return std::any_cast<T>(m_items[hash]);
	}

	template<class T = bool>
	void set( uint32_t hash, T value ) {
		m_items[ hash ] = value;
	}
}