#pragma once
#include "../../visuals/chams.h"
namespace shared_chams
{
	bool __stdcall handler( size_t message_size, const char* message )
	{
			SharedChams chams_message;

			chams_entity_settings_t settings;

			/* parse message*/
			chams_message.ParseFromArray( message, message_size );
			auto invisible_message = chams_message.invisible( );
			{


				settings.m_invisible.m_enable = invisible_message.enable( );

				for ( auto i = 0; i < 6; i++ )
				{
					settings.m_invisible.m_materials[ i ].m_enable = invisible_message.materials( ).Get( i ).enable( );
					settings.m_invisible.m_materials[ i ].m_material = invisible_message.materials( ).Get( i ).material( );
					settings.m_invisible.m_materials[ i ].m_color = col_t( invisible_message.materials( ).Get( i ).color_r( ), invisible_message.materials( ).Get( i ).color_g( )
					, invisible_message.materials( ).Get( i ).color_b( ) ) ;
				}

			}

			auto visible_message = chams_message.visible( );
			{


				settings.m_visible.m_enable = visible_message.enable( );

				for ( auto i = 0; i < 6; i++ )
				{
					settings.m_visible.m_materials[ i ].m_enable = visible_message.materials( ).Get( i ).enable( );
					settings.m_visible.m_materials[ i ].m_material = visible_message.materials( ).Get( i ).material( );
					settings.m_visible.m_materials[ i ].m_color = col_t( visible_message.materials( ).Get( i ).color_r( ), visible_message.materials( ).Get( i ).color_g( )
					, visible_message.materials( ).Get( i ).color_b( ) );
				}

			}

			chams->set_shared_chams( chams_message.player_index( ), settings );
			return true;
	}

	void send_data( )
	{
		if ( !globals::m_local || !globals::m_local->is_alive( ) || !interfaces::m_engine->is_in_game( )  )
			return;

		SharedChams message;

		/* fill message*/
		message.set_player_index( globals::m_local->get_index( ) );

		ChamsSettings settings_visible;

		auto player_settings = cfg::get < chams_entity_settings_t >( FNV1A( "chams.local_player" ) );

		settings_visible.set_enable( player_settings.m_visible.m_enable );

		for ( auto& material : player_settings.m_visible.m_materials )
		{
			auto message_material = settings_visible.add_materials( );

			message_material->set_enable( material.m_enable );

			message_material->set_material( material.m_material );

			message_material->set_color_r( material.m_color.r() );
			message_material->set_color_g( material.m_color.g( ) );
			message_material->set_color_b( material.m_color.b( ) );

		}
		ChamsSettings settings_invisible;


		settings_invisible.set_enable( player_settings.m_invisible.m_enable );

		for ( auto& material : player_settings.m_invisible.m_materials )
		{
			auto message_material = settings_invisible.add_materials( );

			message_material->set_enable( material.m_enable );

			message_material->set_material( material.m_material );

			message_material->set_color_r( material.m_color.r( ) );
			message_material->set_color_g( material.m_color.g( ) );
			message_material->set_color_b( material.m_color.b( ) );

		}
		message.set_allocated_invisible( &settings_invisible );

		shared->m_socket->send_message_to_all( steamsockets::message_type_t::SHARED_CHAMS, &message );
	}
}
