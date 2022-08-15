#pragma once
#include "json/json.hpp"

namespace cfg {
	void init( );

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