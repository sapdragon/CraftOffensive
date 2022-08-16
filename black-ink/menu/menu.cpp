#include "menu.h"

#include "notifies.h"
#include "elements.h"

#include "../api/api.h"

std::string tabLabels[ 6 ] = { _( "Legit" ), _( "Visuals" ), _( "Misc" ), _( "Skins" ), _( "Files" ), _( "Dashboard" ) };

static char login[ 32 ];
static char password[ 32 ];
static char email[ 32 ];

void user( ) {
	ImGui::Begin( _( "123" ), 0 );
	{
		ImGui::SetWindowSize( ImVec2( 400, 400 ), ImGuiCond_Once );


		static int stage = 0;

		if ( ImGui::Button( "auth" ) ) stage = 0;
		ImGui::SameLine( );
		if ( ImGui::Button( "register" ) ) stage = 1;

		if ( stage == 0 ) {
			ImGui::InputText( "login", login, 32 );
			ImGui::InputText( "password", password, 32 );

			if ( ImGui::Button( "Sign In" ) ) {
				cloud->sigin( login, password );
			}
		}

		if ( stage == 1 ) {
			ImGui::InputText( "login", login, 32 );
			ImGui::InputText( "password", password, 32 );
			ImGui::InputText( "email", email, 32 );

			if ( ImGui::Button( "Sign Up" ) ) {
				cloud->signup( login, password, email );
			}
		}
	}
	ImGui::End( );
}

void c_menu::on_paint() {
	notifies::handle( ImGui::GetForegroundDrawList( ) );

	if (!(input::m_blocked = input::get_key<TOGGLE>(VK_INSERT)))
		return;

	ImGui::Begin( _( "Back" ), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus );
	{
		ImGui::SetWindowSize( ImGui::GetIO( ).DisplaySize );
		ImGui::SetWindowPos( {} );

		ImGui::GetWindowDrawList( )->AddRectFilled( { 0, 0 }, ImGui::GetIO( ).DisplaySize, ImColor( 24, 24, 24, 100 ) );
		ImGui::GetWindowDrawList( )->AddImage( assets::logotype,
			ImGui::GetIO( ).DisplaySize / 2 - ImVec2( 367, 51 ),
			ImGui::GetIO( ).DisplaySize / 2 + ImVec2( 367, 51 )
		);
	}
	ImGui::End( );

	if ( !cloud->user_profile.m_authorized ) {
		user( );
	}
	else {
		ImGui::Begin( _( "CraftOffensive" ), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration );
		{
			ImGui::SetWindowSize( ImVec2( 660, 445 ), ImGuiCond_Once );

			auto draw = ImGui::GetWindowDrawList( );
			auto pos = ImGui::GetWindowPos( );

			draw->AddRectFilled( pos, pos + ImVec2( 660, 50 ), ImColor( 26, 26, 26 ) );
			draw->AddImage( assets::background, pos + ImVec2( 0, 50 ), pos + ImVec2( 600, 475 ) );
			draw->AddImage( assets::background, pos + ImVec2( 600, 50 ), pos + ImVec2( 1200, 475 ) );

			for ( auto a = 0; a < 6; a++ ) {
				ImGui::SetCursorPos( { float( a * 110 ), 0 } );
				elements::tab( tabLabels[ a ], assets::icons[ a ], m_selected_tab, a );
			}

			draw->AddRectFilled( pos + ImVec2( 0, 50 ), pos + ImVec2( 660, 80 ), ImColor( 65, 65, 65, 210 ) );

			ImGui::SetCursorPos( { 0, 50 } );
			ImGui::BeginGroup( );
			{
				if ( m_selected_tab == 0 ) {
					elements::subtab( _( "Aimbot" ), { 330, 30 }, m_selected_subtab[ 0 ], 0 );
					ImGui::SameLine( );
					elements::subtab( _( "Triggerbot" ), { 330, 30 }, m_selected_subtab[ 0 ], 1 );
				}
			}
			ImGui::EndGroup( );

			ImGui::SetCursorPos( { 15, 95 } );
			ImGui::BeginGroup( );
			{
				if ( m_selected_tab == 0 ) {
					elements::child( _( "General" ), { 220, 220 }, [ ] ( ) {
						elements::checkbox( _( "auto_jump" ), FNV1A( "auto_jump" ) );
						elements::slider_int( _( "slider" ), FNV1A( "slider" ), 30, 120, "Slider Value: %i" );
						if ( elements::button( _( "Button" ), ImVec2( 200, 30 ) ) )
						{
							cloud->get_configs( );
							notifies::push( "CraftOffensive.pw", "Coming soon..." );
						}
					} );
				}
			}
			ImGui::EndGroup( );
		}
		ImGui::End( );
	}
}