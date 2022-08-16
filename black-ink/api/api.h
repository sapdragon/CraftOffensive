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

struct user_t {
	std::string m_username = "";
	std::string m_password = "";
	std::string m_hwid = "";
	
	bool m_authorized = false;
};

class c_cloud_api : public c_singleton<c_cloud_api> {
public:
	void get_configs( );
	void save_config( std::string secure_id );
	void create_config( std::string name );
	void delete_config( std::string secure_id );

	std::vector<config_t> user_configs;

	void sigin( std::string login, std::string password );
	void signup( std::string login, std::string password, std::string email );

	user_t user_profile;
};

#define cloud c_cloud_api::instance()