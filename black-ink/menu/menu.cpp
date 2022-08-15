#include "menu.h"
#include "../utils/render/render.h"

void c_menu::on_paint() {
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

	ImGui::Begin( _( "CraftOffensive" ), 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration );
	{
		ImGui::SetWindowSize( ImVec2( 600, 445 ), ImGuiCond_Once );

		auto draw = ImGui::GetWindowDrawList( );
		auto pos = ImGui::GetWindowPos( );

		draw->AddRectFilled( pos, pos + ImVec2( 600, 50 ), ImColor( 26, 26, 26 ));
		draw->AddImage( assets::background, pos + ImVec2( 0, 50 ), pos + ImVec2( 600, 475 ));
	}
	ImGui::End();
}