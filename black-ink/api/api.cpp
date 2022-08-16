#include "api.h"

#include "../menu/notifies.h"

static int writer( char* data, size_t size, size_t nmemb, std::string* buffer )
{
	int result = 0;
	if ( buffer != NULL ) { buffer->append( data, size * nmemb ); result = size * nmemb; }
	return result;
}

void get_configs_internal( )
{
	std::string szRequest = _( "https://craftoffensive.pw/v1/cloud/getConfigs" );
	std::string request_body = "hwid=" + cloud->user_profile.m_hwid;
	std::string szResponse;

	curl_global_init( CURL_GLOBAL_ALL );
	auto inited_curl = curl_easy_init( );

	auto headers = curl_slist_append( NULL, _( "Expect:" ) );
	curl_easy_setopt( inited_curl, CURLOPT_HTTPHEADER, headers );
	curl_easy_setopt( inited_curl, CURLOPT_URL, szRequest.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_POST, 1 );
	curl_easy_setopt( inited_curl, CURLOPT_POSTFIELDS, request_body.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEFUNCTION, writer );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEDATA, &szResponse );
	curl_easy_setopt( inited_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 );

	curl_easy_perform( inited_curl );

	if ( szResponse.empty( ) )
		return;

	if ( !nlohmann::json::accept( szResponse ) )
		return;

	auto json_resp = nlohmann::json::parse( szResponse );

	if ( json_resp[ _( "Status" ) ] == _( "Failure" ) )
	{
		notifies::push( _( "Cloud Configs" ), _( "Something went wrong." ) );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Empty" ) )
	{
		cloud->user_configs.clear( );

		notifies::push( _( "Cloud Configs" ), _( "No cloud configs founded." ) );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
	{
		cloud->user_configs.clear( );

		auto config_amount = json_resp[ "Configs" ].size( );
		if ( config_amount <= 0 )
			return;

		for ( auto a = 0; a < config_amount; a++ ) {
			cloud->user_configs.push_back( config_t{
				.m_name = json_resp[ "Configs" ][ a ][ "Name" ].get<std::string>( ),
				.m_author = json_resp[ "Configs" ][ a ][ "Author" ].get<std::string>( ),
				.m_owner = json_resp[ "Configs" ][ a ][ "Owner" ].get<std::string>( ),
				.m_secure_id = json_resp[ "Configs" ][ a ][ "SecureID" ].get<std::string>( ),
				.m_data = json_resp[ "Configs" ][ a ][ "Content" ].get<std::string>( ),
				.m_deleted = json_resp[ "Configs" ][ a ][ "Deleted" ].get<bool>( ),
			} );
		}

		notifies::push( _( "Cloud Configs" ), _( "Configs was successfully fetched." ) );
	}

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );
}

void save_config_internal( std::string secure_id )
{
	std::string request_body = "hwid=" + cloud->user_profile.m_hwid + "&secureid=" + secure_id + "&content=" + secure_id;
	std::string szRequest = _( "https://craftoffensive.pw/v1/cloud/saveConfig" );
	std::string szResponse;

	curl_global_init( CURL_GLOBAL_ALL );
	auto inited_curl = curl_easy_init( );

	auto headers = curl_slist_append( NULL, _( "Expect:" ) );
	curl_easy_setopt( inited_curl, CURLOPT_HTTPHEADER, headers );
	curl_easy_setopt( inited_curl, CURLOPT_URL, szRequest.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_POST, 1 );
	curl_easy_setopt( inited_curl, CURLOPT_POSTFIELDS, request_body.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEFUNCTION, writer );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEDATA, &szResponse );
	curl_easy_setopt( inited_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 );

	curl_easy_perform( inited_curl );

	if ( szResponse.empty( ) )
		return;

	if ( !nlohmann::json::accept( szResponse ) )
		return;

	auto json_resp = nlohmann::json::parse( szResponse );

	if ( json_resp[ _( "Status" ) ] == _( "Failure" ) )
	{
		notifies::push( _( "Cloud Configs" ), _( "Something went wrong." ) );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
	{
		notifies::push( _( "Cloud Configs" ), _( "Config was successfully deleted." ) );
	}

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );
}

void create_config_internal( std::string name )
{
	std::string request_body = "hwid=" + cloud->user_profile.m_hwid + "&name=" + name + "&content=";
	std::string szRequest = _( "https://craftoffensive.pw/v1/cloud/createConfig" );
	std::string szResponse;

	curl_global_init( CURL_GLOBAL_ALL );
	auto inited_curl = curl_easy_init( );

	auto headers = curl_slist_append( NULL, _( "Expect:" ) );
	curl_easy_setopt( inited_curl, CURLOPT_HTTPHEADER, headers );
	curl_easy_setopt( inited_curl, CURLOPT_URL, szRequest.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_POST, 1 );
	curl_easy_setopt( inited_curl, CURLOPT_POSTFIELDS, request_body.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEFUNCTION, writer );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEDATA, &szResponse );
	curl_easy_setopt( inited_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 );

	curl_easy_perform( inited_curl );

	if ( szResponse.empty( ) )
		return;

	if ( !nlohmann::json::accept( szResponse ) )
		return;

	auto json_resp = nlohmann::json::parse( szResponse );

	if ( json_resp[ _( "Status" ) ] == _( "Failure" ) )
	{
		notifies::push( _( "Cloud Configs" ), _( "Something went wrong." ) );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
	{
		notifies::push( _( "Cloud Configs" ), _( "Config was successfully created." ) );
	}

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );
}

void delete_config_internal( std::string secure_id )
{
	std::string request_body = "hwid=" + cloud->user_profile.m_hwid + "&secureid=" + secure_id;
	std::string szRequest = _( "https://craftoffensive.pw/v1/cloud/deleteConfig" );
	std::string szResponse;

	curl_global_init( CURL_GLOBAL_ALL );
	auto inited_curl = curl_easy_init( );

	auto headers = curl_slist_append( NULL, _( "Expect:" ) );
	curl_easy_setopt( inited_curl, CURLOPT_HTTPHEADER, headers );
	curl_easy_setopt( inited_curl, CURLOPT_URL, szRequest.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_POST, 1 );
	curl_easy_setopt( inited_curl, CURLOPT_POSTFIELDS, request_body.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEFUNCTION, writer );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEDATA, &szResponse );
	curl_easy_setopt( inited_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 );

	curl_easy_perform( inited_curl );

	if ( szResponse.empty( ) )
		return;

	if ( !nlohmann::json::accept( szResponse ) )
		return;

	auto json_resp = nlohmann::json::parse( szResponse );

	if ( json_resp[ _( "Status" ) ] == _( "Failure" ) )
	{
		notifies::push( _( "Cloud Configs" ), _( "Something went wrong." ) );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
	{
		notifies::push( _( "Cloud Configs" ), _( "Config was successfully deleted." ) );
	}

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );
}

void sigin_internal( std::string login, std::string password )
{
	std::string request_body = "hwid=" + cloud->user_profile.m_hwid + "&login=" + login + "&password=" + password;
	std::string szRequest = _( "https://craftoffensive.pw/v1/auth/login" );
	std::string szResponse;

	curl_global_init( CURL_GLOBAL_ALL );
	auto inited_curl = curl_easy_init( );

	auto headers = curl_slist_append( NULL, _( "Expect:" ) );
	curl_easy_setopt( inited_curl, CURLOPT_HTTPHEADER, headers );
	curl_easy_setopt( inited_curl, CURLOPT_URL, szRequest.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_POST, 1 );
	curl_easy_setopt( inited_curl, CURLOPT_POSTFIELDS, request_body.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEFUNCTION, writer );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEDATA, &szResponse );
	curl_easy_setopt( inited_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 );

	curl_easy_perform( inited_curl );

	if ( szResponse.empty( ) )
		return;

	if ( !nlohmann::json::accept( szResponse ) )
		return;

	auto json_resp = nlohmann::json::parse( szResponse );

	if ( json_resp[ _( "Status" ) ] == _( "Empty fields" ) )
	{
		notifies::push( _( "Authorization" ), _( "Error" ) );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "OK" ) )
	{
		cloud->user_profile.m_authorized = true;
		cloud->user_profile.m_username = login;
		cloud->user_profile.m_password = password;

		get_configs_internal( );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "User not found" ) )
	{
		notifies::push( _( "Authorization" ), _( "User not found" ) );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Wrong password" ) )
	{
		notifies::push( _( "Authorization" ), _( "Wrong password" ) );
	}

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );
}

void signup_internal( std::string login, std::string password, std::string email ) {
	std::string request_body = "email=" + email + "&hwid=" + cloud->user_profile.m_hwid + "&login=" + login + "&password=" + password;
	std::string szRequest = _( "https://craftoffensive.pw/v1/auth/register" );
	std::string szResponse;

	curl_global_init( CURL_GLOBAL_ALL );
	auto inited_curl = curl_easy_init( );

	auto headers = curl_slist_append( NULL, _( "Expect:" ) );
	curl_easy_setopt( inited_curl, CURLOPT_HTTPHEADER, headers );
	curl_easy_setopt( inited_curl, CURLOPT_URL, szRequest.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_POST, 1 );
	curl_easy_setopt( inited_curl, CURLOPT_POSTFIELDS, request_body.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEFUNCTION, writer );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEDATA, &szResponse );
	curl_easy_setopt( inited_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 );

	curl_easy_perform( inited_curl );

	if ( szResponse.empty( ) )
		return;

	if ( !nlohmann::json::accept( szResponse ) )
		return;

	auto json_resp = nlohmann::json::parse( szResponse );

	if ( json_resp[ _( "Status" ) ] == _( "Empty fields" ) )
	{
		notifies::push( "Authorization", "Error" );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Wrong email" ) )
	{
		notifies::push( "Authorization", "Wrong email" );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "OK" ) )
	{
		cloud->user_profile.m_authorized = true;
		cloud->user_profile.m_username = login;
		cloud->user_profile.m_password = password;

		get_configs_internal( );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "User not found" ) )
	{
		notifies::push( _( "Authorization" ), _( "User not found" ) );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Wrong password" ) )
	{
		notifies::push( _( "Authorization" ), _( "Wrong password" ) );
	}

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );
}

void c_cloud_api::get_configs( )
{
	std::thread( get_configs_internal ).detach( );
}

void c_cloud_api::save_config( std::string secure_id )
{
	std::thread( save_config_internal, secure_id ).detach( );
}

void c_cloud_api::create_config( std::string name )
{
	std::thread( create_config_internal, name ).detach( );
}

void c_cloud_api::delete_config( std::string secure_id )
{
	std::thread( delete_config_internal, secure_id ).detach( );
}

void c_cloud_api::sigin( std::string login, std::string password )
{
	std::thread( sigin_internal, login, password ).detach( );
}

void c_cloud_api::signup( std::string login, std::string password, std::string email )
{
	std::thread( signup_internal, login, password, email ).detach( );
}
