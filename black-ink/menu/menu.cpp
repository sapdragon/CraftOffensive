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
			elements::tab( _( "Players" ), assets::icons[ 1 ], m_selected_tab, 1 );
			elements::tab( _( "Weapons" ), assets::icons[ 2 ], m_selected_tab, 2 );
			elements::tab( _( "World" ), assets::icons[ 3 ], m_selected_tab, 3 );

			ImGui::NewLine( );
			ImGui::NewLine( );
			ImGui::NewLine( );

			draw->AddText( pos + ImVec2( 16, 122 + 132 ), ImColor( 255, 255, 255, 255 ), _( "Miscellaneous" ) );
			elements::tab( _( "Movement" ), assets::icons[ 4 ], m_selected_tab, 4 );
			elements::tab( _( "Visuals" ), assets::icons[ 5 ], m_selected_tab, 5 );

			ImGui::NewLine( );
			ImGui::NewLine( );
			ImGui::NewLine( );

			draw->AddText( pos + ImVec2( 16, 122 + 235 ), ImColor( 255, 255, 255, 255 ), _( "Files" ) );
			elements::tab( _( "Configurations" ), assets::icons[ 6 ], m_selected_tab, 6 );
			elements::tab( _( "Chams Materials" ), assets::icons[ 7 ], m_selected_tab, 7 );
		}
		ImGui::EndGroup( );

		if ( m_selected_tab != 6 ) {
			ImGui::SetCursorPos( {170, 10} );
			elements::config_save( );
		}

		if ( m_selected_tab == 6 ) {
			static char config_name[ 24 ];

			ImGui::SetCursorPos( { 170, 10 } );
			ImGui::InputText( _( "Configuration name" ), config_name, 24, ImGuiInputTextFlags_Big );

			ImGui::SetCursorPos( { 380, 10 } );
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

		if ( m_selected_tab == 1 ) {
			static std::vector<std::string> tabs = { "Enemy", "Team" };

			ImGui::SetCursorPos( { 260, 10 } );
			elements::subtabs( "tabs", tabs, m_selected_subtab[ 1 ] );
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
			if ( m_selected_tab == 1 ) {
				ImGui::BeginGroup( );
				{
					elements::child_internal( _( "ESP" ), { 215, 70 } );
					{
						elements::checkbox( _( "Shared ESP" ), FNV1A( "shared.esp" ) );
					}
					elements::child_end_internal( );
				}
				ImGui::EndGroup( );


				if ( m_selected_subtab[ 1 ] == 0 ) {
					ImGui::SameLine( 225 );

					ImGui::BeginGroup( );
					{
						elements::child_internal( _( "Chams Visible" ), { 215, 150 } );
						{
							elements::checkbox_bool( _( "Enemy Visible" ), cfg::enemy_visible.m_enable );

							if ( ImGui::BeginCombo( "Material", chams->materials[ cfg::enemy_visible.m_material ].label.c_str( ) ) )
							{
								for ( auto a = 0; a < chams->materials.size( ); a++ ) {
									if ( ImGui::Selectable( chams->materials[ a ].label.c_str( ) ) )
										cfg::enemy_visible.m_material = a;
								}

								ImGui::EndCombo( );
							}

							elements::color_edit4( "Color", &cfg::enemy_visible.m_color, NULL );
						}
						elements::child_end_internal( );

						ImGui::NewLine( );

						elements::child_internal( _( "Chams Invisible" ), { 215, 150 } );
						{
							static int material = 0;

							elements::checkbox_bool( _( "Enemy Invisible" ), cfg::enemy_invisible.m_enable );

							if ( ImGui::BeginCombo( "Material", chams->materials[ cfg::enemy_invisible.m_material ].label.c_str( ) ) )
							{
								for ( auto a = 0; a < chams->materials.size( ); a++ ) {
									if ( ImGui::Selectable( chams->materials[ a ].label.c_str( ) ) )
										cfg::enemy_invisible.m_material = a;
								}

								ImGui::EndCombo( );
							}

							elements::color_edit4( "Color", &cfg::enemy_invisible.m_color, NULL );
						}
						elements::child_end_internal( );
					}
					ImGui::EndGroup( );
				}
				if ( m_selected_subtab[ 1 ] == 1 ) {
					ImGui::SameLine( 225 );

					ImGui::BeginGroup( );
					{
						elements::child_internal( _( "Chams Visible" ), { 215, 150 } );
						{
							elements::checkbox_bool( _( "Team Visible" ), cfg::teammates_visible.m_enable );

							if ( ImGui::BeginCombo( "Material", chams->materials[ cfg::teammates_visible.m_material ].label.c_str( ) ) )
							{
								for ( auto a = 0; a < chams->materials.size( ); a++ ) {
									if ( ImGui::Selectable( chams->materials[ a ].label.c_str( ) ) )
										cfg::teammates_visible.m_material = a;
								}

								ImGui::EndCombo( );
							}

							elements::color_edit4( "Color", &cfg::teammates_visible.m_color, NULL );
						}
						elements::child_end_internal( );

						ImGui::NewLine( );

						elements::child_internal( _( "Chams Invisible" ), { 215, 150 } );
						{
							static int material = 0;

							elements::checkbox_bool( _( "Team Invisible" ), cfg::teammates_invisible.m_enable );

							if ( ImGui::BeginCombo( "Material", chams->materials[ cfg::teammates_invisible.m_material ].label.c_str( ) ) )
							{
								for ( auto a = 0; a < chams->materials.size( ); a++ ) {
									if ( ImGui::Selectable( chams->materials[ a ].label.c_str( ) ) )
										cfg::teammates_invisible.m_material = a;
								}

								ImGui::EndCombo( );
							}

							elements::color_edit4( "Color", &cfg::teammates_invisible.m_color, NULL );
						}
						elements::child_end_internal( );
					}
					ImGui::EndGroup( );
				}
			}
			if ( m_selected_tab == 4 ) {
				ImGui::BeginGroup( );
				{
					elements::child_internal( _( "Jumps" ), { 215, 100 } );
					{
						elements::checkbox( _( "Bunnyhop" ), FNV1A( "auto_jump" ) );
						elements::checkbox( _( "Auto-Strafer" ), FNV1A( "autotrafe" ) );
					}
					elements::child_end_internal( );
				}
				ImGui::EndGroup( );

				ImGui::SameLine( 225 );

				ImGui::BeginGroup( );
				{
					elements::child_internal( _( "Fake Lags" ), { 215, 103 } );
					{
						elements::checkbox( _( "Enable" ), FNV1A( "fakelags.enable" ) );
						elements::slider_int( _( "Amount" ), FNV1A( "fakelags.amount" ), 0, 14, "Fake Lag Amount: %i" );
					}
					elements::child_end_internal( );
				}
				ImGui::EndGroup( );
			}
			if ( m_selected_tab == 5 ) {
				ImGui::BeginGroup( );
				{
					elements::child_internal( _( "View Model" ), { 200, 223 } );
					{
						elements::checkbox( _( "Enable" ), FNV1A( "misc.view_model.enable" ) );
						elements::checkbox( _( "Override in scope" ), FNV1A( "misc.view_model.override_while_scoped" ) );
						elements::slider_int( _( "FOV" ), FNV1A( "misc.view_model.fov" ), 60, 130, "FOV: %i" );

						elements::slider_float( _( "X" ), FNV1A( "misc.view_model.x" ), -3, 3, "X: %0.1f" );
						elements::slider_float( _( "Y" ), FNV1A( "misc.view_model.y" ), -3, 3, "Y: %0.1f" );
						elements::slider_float( _( "Z" ), FNV1A( "misc.view_model.z" ), -3, 3, "Z: %0.1f" );
					}
					elements::child_end_internal( );
				}
				ImGui::EndGroup( );

				ImGui::SameLine( 225 );

				ImGui::BeginGroup( );
				{

				}
				ImGui::EndGroup( );
			}
 			if ( m_selected_tab == 6 ) {
				for ( auto conf : cloud->user_configs ) {
					elements::config( conf );
					ImGui::NewLine( );
				}
			}
			if ( m_selected_tab == 7 ) {
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
		draw->AddLine( pos + ImVec2( 160, 50 ), pos + ImVec2( 630, 50 ), ImColor( 0, 0, 0, 255 ) );
	}
	ImGui::End( );

	if ( m_selected_tab == 1 && (m_selected_subtab[ 1 ] == 0 || m_selected_subtab[ 1 ]  == 1))
	{
		ImGui::Begin( "preview", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBringToFrontOnFocus );
		{
			ImGui::SetWindowSize( { 330, 440 } );
			ImGui::SetWindowPos( pos + ImVec2{ 640, 30 } );

			auto draw_model = ImGui::GetWindowDrawList( );
			auto pos_model = ImGui::GetWindowPos( );
			
			static bool open_edit_menu = false;

			draw_model->AddImageRounded( assets::background, pos_model, pos_model + ImVec2( 330, 440 ), {}, {1, 1}, ImColor(255, 255, 255), 8 );
			draw_model->AddRect( pos_model, pos_model + ImVec2( 330, 440 ), ImColor( 0, 0, 0, 255 ), 8 );

			if ( g_Model.get_preview_texture() != nullptr )
			{
				draw_model->PushClipRect( ImGui::GetWindowPos( ) + ImVec2( 0, 0 ), ImGui::GetWindowPos( ) + ImVec2( 165, 440 ), true );
				draw_model->AddImage(
					g_Model.get_preview_texture( )->m_handles[ 0 ]->m_texture,
					ImGui::GetWindowPos( ) - ImVec2(0, 30),
					ImGui::GetWindowPos( ) + ImVec2( g_Model.get_preview_texture( )->get_actual_width( ), g_Model.get_preview_texture( )->get_actual_height( ) - 30 ),
					ImVec2( 0, 0 ), ImVec2( 1, 1 ),
					ImColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
				draw_model->PopClipRect( );

			}

			if ( g_Model.get_text_preview_texture( ) != nullptr )
			{
				draw_model->PushClipRect( ImGui::GetWindowPos( ) + ImVec2( 165, 0 ), ImGui::GetWindowPos( ) + ImVec2( 330, 440 ), true );
				draw_model->AddImage(
					g_Model.get_text_preview_texture( )->m_handles[ 0 ]->m_texture,
					ImGui::GetWindowPos( ) - ImVec2( 0, 30 ),
					ImGui::GetWindowPos( ) + ImVec2( g_Model.get_text_preview_texture( )->get_actual_width( ), g_Model.get_text_preview_texture( )->get_actual_height( ) - 30 ),
					ImVec2( 0, 0 ), ImVec2( 1, 1 ),
					ImColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
				draw_model->PopClipRect( );
			}

			draw_model->AddText( pos_model + ImVec2( 165 - ImGui::CalcTextSize( "Open widgets menu" ).x/2, 415 ), ImColor( 255, 255, 255 ), "Open widgets menu" );

			ImGui::SetCursorPos( { 0, 410 } );
			if ( ImGui::InvisibleButton( "Swap", { 330, 30 } ) )
				open_edit_menu = true;

			float size = m_animator.ValueInSine( "widgets menu", open_edit_menu, 0.f, 1.f, 0.05f );

			if ( size >= 0.12f ) {
	
				ImGui::Begin( "items", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar );
				{
					ImGui::SetWindowSize( { 330, float(int(100 * size)) } );
					ImGui::SetWindowPos( pos + ImVec2{ 640, 470 - 100 * size } );

					auto drawitems = ImGui::GetWindowDrawList( );
					auto positems = ImGui::GetWindowPos( );

					drawitems->AddRectFilled( positems, positems + ImVec2( 330, ImGui::GetWindowSize().y ), ImColor( 25, 25, 25, 255 ), 8 );
					drawitems->AddRect( positems, positems + ImVec2( 330, ImGui::GetWindowSize( ).y ), ImColor( 0, 0, 0, 255 ), 8 );
					drawitems->AddText( positems + ImVec2(12, 10), ImColor(255, 255, 255), "ESP Items" );

					ImGui::SetCursorPos( { 12, 35 } );
					ImGui::BeginGroup( );
					{
						if ( m_selected_subtab[ 1 ] == 0 ) {
							if ( elements::draggable_tab( "Health", { 55, 25 }, player_esp_preview->get_value( "Health" ) ) )
								player_esp_preview->swap_enable_item( "Health" );

							ImGui::SameLine( 60 );

							if ( elements::draggable_tab( "Armor", { 55, 25 }, player_esp_preview->get_value( "Armor" ) ) )
								player_esp_preview->swap_enable_item( "Armor" );

							ImGui::SameLine( 120 );

							if ( elements::draggable_tab( "Box", { 55, 25 }, player_esp_preview->get_value( "Box" ) ) )
								player_esp_preview->swap_enable_item( "Box" );

							ImGui::SameLine( 180 );

							if ( elements::draggable_tab( "Weapon", { 55, 25 }, player_esp_preview->get_value( "Weapon" ) ) )
								player_esp_preview->swap_enable_item( "Weapon" );

							ImGui::SameLine( 240 );

							if ( elements::draggable_tab( "Username", { 65, 25 }, player_esp_preview->get_value( "Username" ) ) )
								player_esp_preview->swap_enable_item( "Username" );
						}
						else {
							if ( elements::draggable_tab( "Health", { 55, 25 }, player_team_esp_preview->get_value( "Health" ) ) )
								player_team_esp_preview->swap_enable_item( "Health" );

							ImGui::SameLine( 60 );

							if ( elements::draggable_tab( "Armor", { 55, 25 }, player_team_esp_preview->get_value( "Armor" ) ) )
								player_team_esp_preview->swap_enable_item( "Armor" );

							ImGui::SameLine( 120 );

							if ( elements::draggable_tab( "Box", { 55, 25 }, player_team_esp_preview->get_value( "Box" ) ) )
								player_team_esp_preview->swap_enable_item( "Box" );

							ImGui::SameLine( 180 );

							if ( elements::draggable_tab( "Weapon", { 55, 25 }, player_team_esp_preview->get_value( "Weapon" ) ) )
								player_team_esp_preview->swap_enable_item( "Weapon" );

							ImGui::SameLine( 240 );

							if ( elements::draggable_tab( "Username", { 65, 25 }, player_team_esp_preview->get_value( "Username" ) ) )
								player_team_esp_preview->swap_enable_item( "Username" );
						}
					}
					ImGui::EndGroup( );

					if ( !ImGui::IsWindowHovered( ) && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked( 0 ) )
						open_edit_menu = false;
				}
				ImGui::End( );
			}

			if ( m_selected_subtab[ 1 ] == 0  )
				player_esp_preview->instance( );
			else if ( m_selected_subtab[ 1 ] == 1 )
				player_team_esp_preview->instance( );
		}
		ImGui::End( );
	}
}