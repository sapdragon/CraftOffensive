#pragma once
#include "../../globals.h"

#include <vector>
#include <string>
#include <thread>

#include "curl/curl.h"
#include "../utils/json.hpp"

struct config_t {
	std::string m_name;
	std::string m_author;
	std::string m_owner;
	std::string m_secure_id;
	std::string m_data;
	bool m_deleted;
};

class c_cloud_api : public c_singleton<c_cloud_api> {
public:
	void get_configs( );
	void save_config( std::string secure_id );
	void create_config( std::string name );
	void delete_config( std::string secure_id );
	void load_config( std::string secure_id );

	std::vector<config_t> user_configs;
};

inline c_cloud_api* cloud = new c_cloud_api( );