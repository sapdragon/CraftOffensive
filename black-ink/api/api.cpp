#include "api.h"

#include "../menu/notifies.h"

static int writer( char* data, size_t size, size_t nmemb, std::string* buffer )
{
	int result = 0;
	if ( buffer != NULL ) {	buffer->append( data, size * nmemb );result = size * nmemb;}
	return result;
}

void post_request( std::string url, std::string data, std::function<void( std::string resp )> callback ) {
	std::string szRequest = _( "https://craftoffensive.pw/" ) + url;
	std::string szResponse;

	curl_global_init( CURL_GLOBAL_ALL );
	auto inited_curl = curl_easy_init( );
	
	auto headers = curl_slist_append( NULL, _( "Expect:" ) );
	curl_easy_setopt( inited_curl, CURLOPT_HTTPHEADER, headers );
	curl_easy_setopt( inited_curl, CURLOPT_URL, szRequest.c_str( ) );
	curl_easy_setopt( inited_curl, CURLOPT_POST, 1 );
	curl_easy_setopt( inited_curl, CURLOPT_POSTFIELDS, data.c_str() );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEFUNCTION, writer );
	curl_easy_setopt( inited_curl, CURLOPT_WRITEDATA, &szResponse );
	curl_easy_setopt( inited_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4 );

	curl_easy_perform( inited_curl );

	callback( szResponse );

	curl_easy_cleanup( inited_curl );
	curl_slist_free_all( headers );
}

void c_cloud_api::get_configs( )
{
	std::thread( [ & ] ( ) {
		std::string request_body = "hwid=" + user_profile.m_hwid;

		post_request( _( "v1/cloud/saveConfig" ), request_body, [ & ] ( std::string resp ) {
			if ( resp.empty( ) )
				return;

			MessageBoxA( 0, resp.c_str( ), 0, 0 );

			if ( !nlohmann::json::accept( resp ) )
				return;

			auto json_resp = nlohmann::json::parse( resp );

			if ( json_resp[ _( "Status" ) ] == _( "Failure" ) )
			{
				notifies::push( _( "Cloud Configs" ), _( "Something went wrong." ) );
			}
			else if ( json_resp[ _( "Status" ) ] == _( "Empty" ) )
			{
				user_configs.clear( );

				notifies::push( _( "Cloud Configs" ), _( "No cloud configs founded." ) );
			}
			else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
			{
				user_configs.clear( );

				auto config_amount = json_resp[ "Configs" ].size( );
				for ( auto a = 0; a < config_amount; a++ ) {
					user_configs.push_back( config_t{
						.m_name = json_resp[ "Configs" ][ a ][ "Name" ].get<std::string>( ),
						.m_author = json_resp[ "Configs" ][ a ][ "Author" ].get<std::string>(),
						.m_owner = json_resp[ "Configs" ][ a ][ "Owner" ].get<std::string>( ),
						.m_secure_id = json_resp[ "Configs" ][ a ][ "SecureID" ].get<std::string>( ),
						.m_data = json_resp[ "Configs" ][ a ][ "Content" ].get<std::string>( ),
						.m_deleted = json_resp[ "Configs" ][ a ][ "Deleted" ].get<bool>( ),
					} );
				}

				notifies::push( _( "Cloud Configs" ), _( "Configs was successfully fetched." ) );
			}
		} );
	} ).detach( );
}

void c_cloud_api::save_config( std::string secure_id )
{
	std::thread( [ & ] ( ) {
		std::string request_body ="hwid=" + user_profile.m_hwid + "&secureid=" + secure_id +	"&content=" + secure_id;

		post_request( _( "v1/cloud/saveConfig" ), request_body, [ ] ( std::string resp ) {
			if ( resp.empty( ) )
				return;

			if ( !nlohmann::json::accept( resp ) )
				return;

			auto json_resp = nlohmann::json::parse( resp );

			if ( json_resp[ _( "Status" ) ] == _( "Failure" ) )
			{
				notifies::push( _( "Cloud Configs" ), _( "Something went wrong." ) );
			}
			else if ( json_resp[ _( "Status" ) ] == _( "Success" ) )
			{
				notifies::push( _( "Cloud Configs" ), _( "Config was successfully deleted." ) );
			}
		} );

		get_configs( );
	} ).detach( );
}

void c_cloud_api::create_config( std::string name )
{
	std::thread( [&] ( ) 
	{
		std::string request_body = "hwid=" + user_profile.m_hwid + "&name=" + name + "&content=";

		post_request( _( "v1/cloud/createConfig" ), request_body, [ ] ( std::string resp )  {
			if ( resp.empty( ) )
				return;

			if ( !nlohmann::json::accept( resp ) )
				return;

			auto json_resp = nlohmann::json::parse( resp );

			if ( json_resp[ _("Status") ] == _("Failure") ) 
			{
				notifies::push(_("Cloud Configs" ), _("Something went wrong.") );
			}
			else if ( json_resp[ _( "Status" ) ] == _( "Success" ) ) 
			{
				notifies::push( _( "Cloud Configs" ), _( "Config was successfully created." ) );
			}
		});

		get_configs( );
	} ).detach( );
}

void c_cloud_api::delete_config( std::string secure_id )
{
	std::thread( [&] ( ) {
		std::string request_body = "hwid=" + user_profile.m_hwid + "&secureid=" + secure_id;

		post_request( _( "v1/cloud/deleteConfig" ), request_body, [ ] ( std::string resp )  {
			if ( resp.empty( ) )
				return;

			if ( !nlohmann::json::accept( resp ) )
				return;

			auto json_resp = nlohmann::json::parse( resp );

			if ( json_resp[ _("Status") ] == _("Failure") ) 
			{
				notifies::push(_("Cloud Configs" ), _("Something went wrong.") );
			}
			else if ( json_resp[ _( "Status" ) ] == _( "Success" ) ) 
			{
				notifies::push( _( "Cloud Configs" ), _( "Config was successfully deleted." ) );
			}
		});
		get_configs( );
	} ).detach( );
}

void c_cloud_api::sigin( std::string login, std::string password )
{
	std::thread( [ & ] ( ) {
		std::string request_body = "hwid=" + user_profile.m_hwid + "&login=" + login + "&password=" + password;
		MessageBoxA( 0, request_body.c_str( ), 0, 0 );


		post_request( _( "v1/auth/login" ), request_body, [& ] ( std::string resp ) {
			if ( resp.empty( ) )
				return;

			if ( !nlohmann::json::accept( resp ) )
				return;

			auto json_resp = nlohmann::json::parse( resp );

			if ( json_resp[ _( "Status" ) ] == _( "Empty fields" ) )
			{
				notifies::push( _( "Authorization" ), _( "Error" ) );
			}
			else if ( json_resp[ _( "Status" ) ] == _( "OK" ) )
			{
				user_profile.m_authorized = true;
				user_profile.m_username = login;
				user_profile.m_password = password;

				get_configs( );
			}
			else if ( json_resp[ _( "Status" ) ] == _( "User not found" ) )
			{
				notifies::push( _( "Authorization" ), _( "User not found" ) );
			}
			else if ( json_resp[ _( "Status" ) ] == _("Wrong password" ) )
			{
				notifies::push( _( "Authorization" ), _( "Wrong password" ) );
			}
		} );
	} ).detach( );
}

void c_cloud_api::signup( std::string login, std::string password, std::string email )
{
	std::thread( [ & ] ( ) {
		std::string request_body = "email=" + email + "&hwid=" + user_profile.m_hwid + "&login = " + login + "&password = " + password;

		MessageBoxA( 0, request_body.c_str(), 0, 0);

		post_request( _( "v1/auth/register" ), request_body, [& ] ( std::string resp ) {
			if ( resp.empty( ) )
				return;

			if ( !nlohmann::json::accept( resp ) )
				return;

			auto json_resp = nlohmann::json::parse( resp );

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
				user_profile.m_authorized = true;
				user_profile.m_username = login;
				user_profile.m_password = password;

				get_configs( );
			}
			else if ( json_resp[ _( "Status" ) ] == _( "User not found" ) )
			{
				notifies::push( _( "Authorization" ), _( "User not found" ) );
			}
			else if ( json_resp[ _( "Status" ) ] == _( "Wrong password" ) )
			{
				notifies::push( _( "Authorization" ), _( "Wrong password" ) );
			}
		} );
	} ).detach( );
}
