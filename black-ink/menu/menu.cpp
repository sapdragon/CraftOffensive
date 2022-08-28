#include "menu.h"

#include "notifies.h"
#include "elements.h"

#include "../api/api.h"
#include "../features/visuals/model/model.h"
#include "../features/visuals/chams.h"
#include "../features/visuals/preview/preview.hpp"

bool array_have_that_name( std::vector < chams_layer > materials, std::string name ) {
	for ( auto a = 0; a < materials.size( ); a++ ) {
		if ( materials[ a ].label == name )
			return true;
	}

	return false;
}

std::vector<std::string> hitboxes = {
		"Head", "Neck", "Pelvis","Stomach",
		"Lower Chest","Chest","Upper Chest","Right thigh",
		"Left thigh", "Right Calf", "Left Calf", "Right Foot",
		"Left Foot","Right Hand", "Left Hand","Right Upper Arm",
		"Right Forearm", "Left Upper Arm", "Left Forearm"
};

void chams_page( std::vector < chams_material_settings_t>& chams_source ) {
	static char label[ 12 ];

	elements::child( _( "Added Materials" ), { 215, 420 }, [ & ] ( ) {
		ImGui::BeginChild( "Layers", ImVec2( 195, 330 ), false, ImGuiWindowFlags_NoBackground );
		{
			for ( auto mats = 0; mats < chams_source.size( ); mats++ ) {
				if ( elements::chams_item( mats, chams_source ) ) {
					menu->selected_material = mats;
					memset( label, 0, sizeof label );
				}
			}
		}
		ImGui::EndChild( );

		ImGui::SetCursorPos( { 0, 340 } );
		if ( elements::button( _( "New Material" ), ImVec2( 195, 32 ) ) ) {
			chams_source.push_back( chams_material_settings_t{} );
		}
	} );

	if ( !chams_source.empty( ) ) {
		ImGui::SameLine( 225 );

		elements::child( _( "Edit Material" ), { 215, 200 }, [ & ] ( ) {

			if ( ImGui::InputText( "Material Label", label, 12 ) ) {
				chams_source[ menu->selected_material ].label = std::string( label );
			}

			if ( ImGui::BeginCombo( "Material", chams->materials[ chams_source[ menu->selected_material ].m_material ].label.c_str( ) ) )
			{
				for ( auto a = 0; a < chams->materials.size( ); a++ ) {
					if ( ImGui::Selectable( chams->materials[ a ].label.c_str( ), chams_source[ menu->selected_material ].m_material == a ) )
						chams_source[ menu->selected_material ].m_material = a;
				}

				ImGui::EndCombo( );
			}

			elements::color_edit4( "Color", &chams_source[ menu->selected_material ].m_color, NULL );

			ImGui::SetCursorPos( { 0, 120 } );
			if ( elements::button( _( "Delete current material" ), ImVec2( 195, 30 ) ) ) {
				chams_source.erase( chams_source.begin( ) + menu->selected_material );
				menu->selected_material = 0;
			}
		} );
	}
}

void legit_page( aimbot_group_settings& settings )
{
	ImGui::BeginGroup( );
	{
		elements::child_internal( _( "General" ), { 215, 100 } );
		{
			elements::checkbox_bool( _( "Enable" ), settings.enable );
			elements::checkbox_bool( _( "Silent" ), settings.silent );
		}
		elements::child_end_internal( );
		ImGui::NewLine( );
		elements::child_internal( _( "Checks" ), { 215, 125 } );
		{
			elements::checkbox_bool( _( "Flash check" ), settings.flash_check );
			elements::checkbox_bool( _( "Jump check" ), settings.jump_check );
			elements::checkbox_bool( _( "Smoke check" ), settings.smoke_check );
		}
		elements::child_end_internal( );
		ImGui::NewLine( );
		elements::child_internal( _( "Visual" ), { 215, 70 } );
		{
			elements::checkbox( _( "Show FOV" ), FNV1A( "aimbot.show_fov" ) );
		}
		elements::child_end_internal( );
	}
	ImGui::EndGroup( );

	ImGui::SameLine( 225 );

	ImGui::BeginGroup( );
	{
		elements::child_internal( _( "Main" ), { 215, 155 } );
		{
			elements::slider_floats( _( "FOV" ), settings.fov, 1.f, 50, "FOV: %0.1f" );
			elements::slider_floats( _( "Smooth" ), settings.smooth, 1.f, 50, "Smooth: %0.1f" );
			elements::combo( "Hitboxes", "Select hitboxes", hitboxes, settings.hitboxes );
		}
		elements::child_end_internal( );
		ImGui::NewLine( );
		elements::child_internal( _( "RCS" ), { 215, 200 } );
		{
			elements::slider_ints( _( "Start after x shots" ), settings.rcs.after, 1, 30, "Start after %i shots" );
			elements::slider_floats( _( "RCS FOV" ), settings.rcs.fov, 1.f, 50, "FOV: %0.1f" );
			elements::slider_floats( _( "RCS Smooth" ), settings.rcs.smooth, 1.f, 50, "Smooth: %0.1f" );
			elements::slider_floats( _( "RCS Pitch" ), settings.rcs.pitch, 0.f, 1.f, "Pitch: %0.1f" );
			elements::slider_floats( _( "RCS Yaw" ), settings.rcs.yaw, 0.f, 1.f, "Yaw: %0.1f" );
		}
		elements::child_end_internal( );
	}
	ImGui::EndGroup( );
}

void c_menu::on_paint( ) {
	notifies::handle( ImGui::GetForegroundDrawList( ) );

	if ( !( input::m_blocked = input::get_key<TOGGLE>( VK_INSERT ) ) )
		return;

	ImGui::Begin( _( "Back" ), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus );
	{
		ImGui::SetWindowSize( ImGui::GetIO( ).DisplaySize );
		ImGui::SetWindowPos( {} );

		ImGui::GetWindowDrawList( )->AddRectFilled( { 0, 0 }, ImGui::GetIO( ).DisplaySize, ImColor( 24, 24, 24, 25 ) );
		ImGui::GetWindowDrawList( )->AddImage( assets::logotype,
			ImGui::GetIO( ).DisplaySize / 2 - ImVec2( 367, 51 ),
			ImGui::GetIO( ).DisplaySize / 2 + ImVec2( 367, 51 )
		);
	}
	ImGui::End( );

	ImGui::Begin( _( "CraftOffensive" ), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration );
	{
		ImGui::SetWindowSize( ImVec2( 630, 500 ), ImGuiCond_Once );

		draw = ImGui::GetWindowDrawList( );
		pos = ImGui::GetWindowPos( );

		draw->PushClipRect( pos + ImVec2( 160, 0 ), pos + ImVec2( 630, 500 ), true );
		draw->AddImageRounded( assets::background, pos + ImVec2(30, 0), pos + ImVec2( 630, 500 ), {}, {1, 1}, ImColor( 255, 255, 255, 255 ), 8 );
		draw->PopClipRect( );

		draw->PushClipRect( pos, pos + ImVec2( 160, 500 ), true);
		draw->AddRectFilled( pos, pos + ImVec2( 160, 500 ), ImColor( 26, 26, 26, 190 ), 8, ImDrawCornerFlags_Left );
		draw->PopClipRect( );

		draw->AddRectFilled( pos + ImVec2( 160, 0 ), pos + ImVec2( 630, 50 ), ImColor( 26, 26, 26, 190 ), 8, ImDrawCornerFlags_TopRight );
		draw->AddImage( assets::logo_mini, pos + ImVec2( 16, 19 ), pos + ImVec2( 144, 31 ) );

		ImGui::SetCursorPos( {0, 75} );
		ImGui::BeginGroup( );
		{
			draw->AddText( pos + ImVec2(16, 50), ImColor(255, 255, 255, 255), _( "Legitbot" ) );
			elements::tab( _( "Aimbot" ), assets::icons[0], m_selected_tab, 0 );
			
			ImGui::NewLine( );
			ImGui::NewLine( );
			ImGui::NewLine( );

			draw->AddText( pos + ImVec2( 16, 122 ), ImColor( 255, 255, 255, 255 ), _( "Visuals" ) );
			elements::tab( _( "ESP" ), assets::icons[ 1 ], m_selected_tab, 1 );
			elements::tab( _( "Chams" ), assets::icons[ 2 ], m_selected_tab, 2 );

			ImGui::NewLine( );
			ImGui::NewLine( );
			ImGui::NewLine( );

			draw->AddText( pos + ImVec2( 16, 122 + 102 ), ImColor( 255, 255, 255, 255 ), _( "Miscellaneous" ) );
			elements::tab( _( "Movement" ), assets::icons[ 3 ], m_selected_tab, 3 );
			elements::tab( _( "Visuals" ), assets::icons[ 4 ], m_selected_tab, 4 );

			ImGui::NewLine( );
			ImGui::NewLine( );
			ImGui::NewLine( );

			draw->AddText( pos + ImVec2( 16, 122 + 205 ), ImColor( 255, 255, 255, 255 ), _( "Files" ) );
			elements::tab( _( "Configurations" ), assets::icons[ 5 ], m_selected_tab, 5 );
			elements::tab( _( "Chams Materials" ), assets::icons[ 6 ], m_selected_tab, 6 );
		}
		ImGui::EndGroup( );

		if ( m_selected_tab != 5 ) {
			ImGui::SetCursorPos( {170, 10} );
			elements::config_save( );
		}

		if ( m_selected_tab == 5 ) {
			static char config_name[ 24 ];

			ImGui::SetCursorPos( { 170, 10 } );
			ImGui::InputText( _( "Configuration name" ), config_name, 24, ImGuiInputTextFlags_Big );

			ImGui::SetCursorPos( { 350, 10 } );
			if ( elements::button( _( "Create" ), ImVec2( 112, 32 ) ) )
				if ( !std::string( config_name ).empty( ) )
					cloud->create_config( config_name );
				else
					notifies::push( "Cloud Configs", "Configure name is too short" );


			ImGui::SetCursorPos( { 499, 10 } );
			if ( elements::button( _( "Refresh" ), ImVec2( 122, 32 ) ) )
				cloud->get_configs( );
		}

		if ( m_selected_tab == 0 ) {
			static std::vector<std::string> tabs = {"Pistols", "SMGs", "Rifles", "Snipers", "Shotgun", "Heavy"};

			ImGui::SetCursorPos( {260, 10} );
			elements::subtabs( "tabs", tabs, m_selected_subtab[0] );
		}

		if ( m_selected_tab == 2 ) {
			static std::vector<std::string> tabs = { "Local Visible", "Local Invisible", "Enemy Visible", "Enemy Invisible", "Team Visible", "Team Invisible" };

			ImGui::SetCursorPos( { 260, 10 } );
			elements::subtabs( "tabs", tabs, m_selected_subtab[ 2 ] );
		}

		ImGui::SetCursorPos( { 170, 60 } );
		ImGui::BeginChild( "items", {630 - 170, 430}, false, ImGuiWindowFlags_NoBackground );
		{
			if ( m_selected_tab == 0 ) {
				if ( m_selected_subtab[ 0 ] == 0 ) legit_page( cfg::aimbot_pistol );
				if ( m_selected_subtab[ 0 ] == 1 ) legit_page( cfg::aimbot_smg );
				if ( m_selected_subtab[ 0 ] == 2 ) legit_page( cfg::aimbot_rifle );
				if ( m_selected_subtab[ 0 ] == 3 ) legit_page( cfg::aimbot_sniper );
				if ( m_selected_subtab[ 0 ] == 4 ) legit_page( cfg::aimbot_shotgun );
				if ( m_selected_subtab[ 0 ] == 5 ) legit_page( cfg::aimbot_heavy );
			}
			if ( m_selected_tab == 2 ) {
				if ( m_selected_subtab[ 2 ] == 0 ) chams_page( cfg::local_player_visible );
				if ( m_selected_subtab[ 2 ] == 1 ) chams_page( cfg::local_player_invisible );
				if ( m_selected_subtab[ 2 ] == 2 ) chams_page( cfg::enemy_visible );
				if ( m_selected_subtab[ 2 ] == 3 ) chams_page( cfg::enemy_invisible );
				if ( m_selected_subtab[ 2 ] == 4 ) chams_page( cfg::teammates_visible );
				if ( m_selected_subtab[ 2 ] == 5 ) chams_page( cfg::teammates_invisible );
			}
			if ( m_selected_tab == 5 ) {
				for ( auto conf : cloud->user_configs ) {
					elements::config( conf );
				}
			}
			if ( m_selected_tab == 6 ) {
				static chams_layer data;
				static char label[ 12 ];
				const char* shader_list[] = { "VertexLitGeneric", "UnlitGeneric" };
				static int shader = 0;

				elements::child( _( "New Material" ), { 450, 425 }, [ & ] ( ) {

					ImGui::Combo( "Shader", &shader, shader_list, 2 );

					if ( ImGui::InputText( "Material Label", label, 12 ) ) {
						data.label = std::string( label );
					}

					editor.Render( "TextEditor", { 450, 245 } );

					ImGui::Spacing( );

					if ( elements::button( _( "New Material" ), ImVec2( 430, 32 ) ) )
					{
						data.material_data = editor.GetText( );
						data.buildin = false;
						data.label = std::string( label );
						data.file_name = std::string( label ) + ".vmt";
						data.shader_type = shader == 0 ? "VertexLitGeneric" : "UnlitGeneric";

						if ( array_have_that_name( chams->materials, data.label ) == false ) {
							chams->create_material( data );
							data = chams_layer{};
						}
						else {
							notifies::push( "Chams System", "That name already taken" );
						}
					}
				} );
			}
		}
		ImGui::EndChild( );

		draw->AddRect( pos, pos + ImVec2( 630, 500 ), ImColor( 0, 0, 0, 255 ), 8 );
		draw->AddLine( pos + ImVec2(160, 0), pos + ImVec2( 160, 500 ), ImColor( 0, 0, 0, 255 ) );
	}
	ImGui::End( );

	if ( m_selected_subtab[ 1 ] == 0 && m_selected_tab == 1 )
	{
		ImGui::Begin( "preview", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration );
		{
			ImGui::SetWindowSize( { 330, 475 } );
			ImGui::SetWindowPos( pos + ImVec2{ 675, 0 } );

			auto draw_model = ImGui::GetWindowDrawList( );
			auto pos_model = ImGui::GetWindowPos( );

			draw_model->PushClipRect( pos_model, pos_model + ImVec2( 330, 35 ), true );
			draw_model->AddImage( assets::dirt, pos_model, pos_model + ImVec2( 600, 50 ) );
			draw_model->PopClipRect( );

			draw_model->AddImage( assets::background, pos_model + ImVec2( 0, 35 ), pos_model + ImVec2( 600, 475 ) );

			if ( g_Model.get_preview_texture( ) != nullptr )
			{
				draw_model->AddImage(
					g_Model.get_preview_texture( )->m_handles[ 0 ]->m_texture,
					ImGui::GetWindowPos( ),
					ImGui::GetWindowPos( ) + ImVec2( g_Model.get_preview_texture( )->get_actual_width( ), g_Model.get_preview_texture( )->get_actual_height( ) ),
					ImVec2( 0, 0 ), ImVec2( 1, 1 ),
					ImColor( 1.0f, 1.0f, 1.0f, 1.0f ) );

			}

			if ( m_selected_subtab[ 1 ] == 0 && selected_esp_tab == 0 )
				player_esp_preview->instance( );
			else if ( m_selected_subtab[ 1 ] == 0 && selected_esp_tab == 1 )
				player_team_esp_preview->instance( );
		}
		ImGui::End( );
	}
}