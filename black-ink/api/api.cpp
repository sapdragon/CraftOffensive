#include "api.h"

#include "../menu/notifies.h"
#include "HardwareID.hpp"

static int writer( char* data, size_t size, size_t nmemb, std::string* buffer )
{
	int result = 0;
	if ( buffer != NULL ) { buffer->append( data, size * nmemb ); result = size * nmemb; }
	return result;
}

void get_configs_internal( )
{
	std::string szRequest = _( "https://craftoffensive.pw/v1/cloud/getConfigs" );
	std::string request_body = "hwid=" + HWID::HardwareID( );
	std::string szResponse;

	curl_global_init( CURL_GLOBAL_ALL );
	auto inited_curl = curl_easy_init( );

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
		notifies::push( "Cloud Configs", "Something went wrong." );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Empty" ) )
	{
		if ( !cloud->user_configs.empty( ) )
			cloud->user_configs.clear( );

		notifies::push( "Cloud Configs", "No cloud configs founded." );
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
				.m_deleted = json_resp[ "Configs" ][ a ][ "Deleted" ].get<bool>( ),
			} );
		}

		notifies::push( "Cloud Configs", "Configs was successfully fetched." );
	}

	curl_easy_cleanup( inited_curl );
}

void save_config_internal( std::string secure_id )
{
	std::string request_body = "hwid=" + HWID::HardwareID( ) + "&secureid=" + secure_id + "&content=" + cfg::json_action( false );
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
		notifies::push( "Cloud Configs", "Something went wrong." );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
	{
		notifies::push( "Cloud Configs", "Config was successfully saved." );
	}

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );

	get_configs_internal( );
}

void create_config_internal( std::string name )
{
	std::string request_body = "hwid=" + HWID::HardwareID( ) + "&name=" + name + "&content=" + cfg::json_action( false );
	std::string szRequest = _( "https://craftoffensive.pw/v1/cloud/createConfig" );
	std::string szResponse;

	curl_global_init( CURL_GLOBAL_ALL );
	auto inited_curl = curl_easy_init( );

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
		notifies::push( "Cloud Configs", "Something went wrong." );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
	{
		notifies::push( "Cloud Configs", "Config was successfully created." );
	}

	curl_easy_cleanup( inited_curl );

	get_configs_internal( );
}

void delete_config_internal( std::string secure_id )
{
	std::string request_body = "hwid=" + HWID::HardwareID( ) + "&secureid=" + secure_id;
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
		notifies::push( "Cloud Configs", "Something went wrong." );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
	{
		notifies::push( "Cloud Configs", "Config was successfully deleted." );
	}

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );

	get_configs_internal( );
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

void load_config_internal( std::string secure_id )
{
	std::string request_body = "hwid=" + HWID::HardwareID( ) + "&secureid=" + secure_id;
	std::string szRequest = _( "https://craftoffensive.pw/v1/cloud/loadConfig" );
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
		notifies::push( "Cloud Configs", "Something went wrong." );
	}
	else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
	{
		cfg::jsonk = nlohmann::json::parse( json_resp[ _( "Config" ) ].get<std::string>( ) );
		cfg::json_action( true );

		notifies::push( "Cloud Configs", "Config was successfully loaded." );
	}

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );
}

void c_cloud_api::load_config( std::string secure_id )
{
	this->secure_id = secure_id;

	std::thread( load_config_internal, secure_id ).detach( );
}