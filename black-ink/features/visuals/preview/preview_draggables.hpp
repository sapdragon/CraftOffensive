#pragma once

#include "../../../utils/imgui/imgui.h"
#include "../../../utils/imgui/imgui_internal.h"

#include "../../../menu/elements.h"

#include "preview.hpp"

namespace EnemyPreview {
	__forceinline ImColor toColor( col_t color )
	{
		return ImColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	}

	inline void Username( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		ImVec2 ImTextSize = ImGui::CalcTextSize( "Nickname" );

		if ( cond == LEFT_COND ) {
			size = ImVec2( ImTextSize.x + 10, 14 + 6 );
			draw->AddText( pos + ImVec2( 1, size.y / 2 - ImTextSize.y / 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.nickname.color" ) ) ), "Nickname" );
		}

		if ( cond == RIGHT_COND ) {
			size = ImVec2( ImTextSize.x + 11, 14 + 6 );
			draw->AddText( pos + ImVec2( 12, size.y / 2 - ImTextSize.y / 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.nickname.color" ) ) ), "Nickname" );
		}

		if ( cond == TOP_COND || cond == BOT_COND ) {
			size = ImVec2( 110, 14 + 6 );
			draw->AddText( pos + ImVec2( 55 - ImTextSize.x / 2, size.y / 2 - ImTextSize.y / 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.nickname.color" ) ) ), "Nickname" );
		}

		if ( ImGui::BeginPopup( "Username" ) ) {
			elements::child( "Username Settings", { 200, 70 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.enemies.nickname.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Username" );
		}
	}

	inline void Weapon( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		ImVec2 ImTextSize = ImGui::CalcTextSize( "Dual Elites" );

		if ( cond == LEFT_COND ) {
			size = ImVec2( ImTextSize.x + 10, ImTextSize.y + 6 );
			draw->AddText( pos + ImVec2( 1, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.weapon.color" ) ) ), "Dual Elites" );
		}

		if ( cond == RIGHT_COND ) {
			size = ImVec2( ImTextSize.x + 10, ImTextSize.y + 6 );
			draw->AddText( pos + ImVec2( 12, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.weapon.color" ) ) ), "Dual Elites" );
		}

		if ( cond == TOP_COND || cond == BOT_COND ) {
			size = ImVec2( 110, ImTextSize.y + 6 );
			draw->AddText( pos + ImVec2( 55 - ImGui::CalcTextSize( "Dual Elites" ).x / 2, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.weapon.color" ) ) ), "Dual Elites" );
		}

		if ( ImGui::BeginPopup( "Weapons" ) ) {
			elements::child( "Weapons Settings", { 200, 70 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.enemies.weapon.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Weapons" );
		}
	}

	inline void HealthBar( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		if ( cond == LEFT_COND || cond == RIGHT_COND ) {
			size = ImVec2( 6, 230 );
			draw->AddRectFilled( pos + ImVec2( 4, 0 ), pos + size - ImVec2( 4, 0 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.health.border.outside.color" ) ) ) );
			draw->AddRectFilled( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 1 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.health.border.inside.color" ) ) ) );

			draw->AddRectFilled( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 98 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.health.color" ) ) ) );
		}

		if ( cond == TOP_COND || cond == BOT_COND ) {
			size = ImVec2( 110, 10 );
			draw->AddRectFilled( pos + ImVec2( 4, 2 ), pos + size - ImVec2( 4, 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.health.border.outside.color" ) ) ) );
			draw->AddRectFilled( pos + ImVec2( 5, 3 ), pos + size - ImVec2( 5, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.health.border.inside.color" ) ) ) );

			draw->AddRectFilled( pos + ImVec2( 5, 3 ), pos + size - ImVec2( 56, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.health.color" ) ) ) );
		}

		if ( ImGui::BeginPopup( "Health" ) ) {
			elements::child( "Health Settings", { 200, 135 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.enemies.health.color" ), NULL );
				elements::color_edit4( "Color Outline 1", FNV1A( "esp.enemies.health.border.outside.color" ), NULL );
				elements::color_edit4( "Color Outline 2", FNV1A( "esp.enemies.health.border.inside.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Health" );
		}
	}

	inline void ArmorBar( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		if ( cond == LEFT_COND || cond == RIGHT_COND ) {
			size = ImVec2( 6, 230 );
			draw->AddRectFilled( pos + ImVec2( 4, 0 ), pos + size - ImVec2( 4, 0 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.armor.border.outside.color" ) ) ) );
			draw->AddRectFilled( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 1 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.armor.border.inside.color" ) ) ) );

			draw->AddRectFilled( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 98 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.armor.color" ) ) ) );
		}

		if ( cond == TOP_COND || cond == BOT_COND ) {
			size = ImVec2( 110, 10 );
			draw->AddRectFilled( pos + ImVec2( 4, 2 ), pos + size - ImVec2( 4, 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.armor.border.outside.color" ) ) ) );
			draw->AddRectFilled( pos + ImVec2( 5, 3 ), pos + size - ImVec2( 5, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.armor.border.inside.color" ) ) ) );

			draw->AddRectFilled( pos + ImVec2( 5, 3 ), pos + size - ImVec2( 56, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.armor.color" ) ) ) );
		}

		if ( ImGui::BeginPopup( "Armor" ) ) {
			elements::child( "Armor Settings", { 200, 135 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.enemies.armor.color" ), NULL );
				elements::color_edit4( "Color Outline 1", FNV1A( "esp.enemies.armor.border.outside.color" ), NULL );
				elements::color_edit4( "Color Outline 2", FNV1A( "esp.enemies.armor.border.inside.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Armor" );
		}
	}

	inline void Box( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		if ( cond == CENTER_COND ) {
			size = ImVec2( 110, 230 );

			draw->AddRect( pos + ImVec2( 4, 0 ), pos + size - ImVec2( 4, 0 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.box.color" ) ) ) );
			draw->AddRect( pos + ImVec2( 3, -1 ), pos + size - ImVec2( 3, -1 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.box.border.outside.color" ) ) ) );
			draw->AddRect( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 1 ), toColor( cfg::get<col_t>( FNV1A( "esp.enemies.box.border.inside.color" ) ) ) );
		}

		if ( ImGui::BeginPopup( "Box" ) ) {
			elements::child( "Box Settings", { 200, 135 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.enemies.box.color" ), NULL );
				elements::color_edit4( "Color Outline 1", FNV1A( "esp.enemies.box.border.outside.color" ), NULL );
				elements::color_edit4( "Color Outline 2", FNV1A( "esp.enemies.box.border.inside.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Box" );
		}
	}

	inline void GetDrawableEnemies( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond, int var ) {
		switch ( var ) {
		case 0: return Weapon( pressed, draw, pos, size, cond ); break;
		case 1: return Username( pressed, draw, pos, size, cond ); break;
		case 2: return HealthBar( pressed, draw, pos, size, cond ); break;
		case 3: return ArmorBar( pressed, draw, pos, size, cond ); break;
		case 4: return Box( pressed, draw, pos, size, cond ); break;
		}
	}
}

namespace TeamPreview {
	__forceinline ImColor toColor( col_t color )
	{
		return ImColor( color.r( ), color.g( ), color.b( ), color.a( ) );
	}

	inline void UsernameTeammates( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		ImVec2 ImTextSize = ImGui::CalcTextSize( "Nickname" );

		if ( cond == LEFT_COND ) {
			size = ImVec2( ImTextSize.x + 10, 14 + 6 );
			draw->AddText( pos + ImVec2( 1, size.y / 2 - ImTextSize.y / 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.nickname.color" ) ) ), "Nickname" );
		}

		if ( cond == RIGHT_COND ) {
			size = ImVec2( ImTextSize.x + 10, 14 + 6 );
			draw->AddText( pos + ImVec2( 9, size.y / 2 - ImTextSize.y / 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.nickname.color" ) ) ), "Nickname" );
		}

		if ( cond == TOP_COND || cond == BOT_COND ) {
			size = ImVec2( 110, 14 + 6 );
			draw->AddText( pos + ImVec2( 55 - ImTextSize.x / 2, size.y / 2 - ImTextSize.y / 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.nickname.color" ) ) ), "Nickname" );
		}


		if ( ImGui::BeginPopup( "Username" ) ) {
			elements::child( "Username Settings", { 200, 70 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.team.nickname.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Username" );
		}
	}

	inline void WeaponTeammates( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		ImVec2 ImTextSize = ImGui::CalcTextSize( "Dual Elites" );

		if ( cond == LEFT_COND ) {
			size = ImVec2( ImTextSize.x + 10, ImTextSize.y + 6 );
			draw->AddText( pos + ImVec2( 1, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.weapon.color" ) ) ), "Dual Elites" );
		}

		if ( cond == RIGHT_COND ) {
			size = ImVec2( ImTextSize.x + 10, ImTextSize.y + 6 );
			draw->AddText( pos + ImVec2( 9, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.weapon.color" ) ) ), "Dual Elites" );
		}

		if ( cond == TOP_COND || cond == BOT_COND ) {
			size = ImVec2( 110, ImTextSize.y + 6 );
			draw->AddText( pos + ImVec2( 55 - ImGui::CalcTextSize( "Dual Elites" ).x / 2, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.weapon.color" ) ) ), "Dual Elites" );
		}

		if ( ImGui::BeginPopup( "Weapons" ) ) {
			elements::child( "Weapons Settings", { 200, 70 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.team.weapon.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Weapons" );
		}
	}

	inline void HealthBarTeammates( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		if ( cond == LEFT_COND || cond == RIGHT_COND ) {
			size = ImVec2( 6, 230 );
			draw->AddRectFilled( pos + ImVec2( 4, 0 ), pos + size - ImVec2( 4, 0 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.health.border.outside.color" ) ) ) );
			draw->AddRectFilled( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 1 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.health.border.inside.color" ) ) ) );

			draw->AddRectFilled( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 98 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.health.color" ) ) ) );
		}

		if ( cond == TOP_COND || cond == BOT_COND ) {
			size = ImVec2( 110, 10 );
			draw->AddRectFilled( pos + ImVec2( 4, 2 ), pos + size - ImVec2( 4, 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.health.border.outside.color" ) ) ) );
			draw->AddRectFilled( pos + ImVec2( 5, 3 ), pos + size - ImVec2( 5, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.health.border.inside.color" ) ) ) );

			draw->AddRectFilled( pos + ImVec2( 5, 3 ), pos + size - ImVec2( 56, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.health.color" ) ) ) );
		}

		if ( ImGui::BeginPopup( "Health" ) ) {
			elements::child( "Health Settings", { 200, 135 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.team.health.color" ), NULL );
				elements::color_edit4( "Color Outline 1", FNV1A( "esp.team.health.border.outside.color" ), NULL );
				elements::color_edit4( "Color Outline 2", FNV1A( "esp.team.health.border.inside.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Health" );
		}
	}

	inline void ArmorBarTeammates( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		if ( cond == LEFT_COND || cond == RIGHT_COND ) {
			size = ImVec2( 6, 230 );
			draw->AddRectFilled( pos + ImVec2( 4, 0 ), pos + size - ImVec2( 4, 0 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.armor.border.outside.color" ) ) ) );
			draw->AddRectFilled( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 1 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.armor.border.inside.color" ) ) ) );

			draw->AddRectFilled( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 98 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.armor.color" ) ) ) );
		}

		if ( cond == TOP_COND || cond == BOT_COND ) {
			size = ImVec2( 110, 10 );
			draw->AddRectFilled( pos + ImVec2( 4, 2 ), pos + size - ImVec2( 4, 2 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.armor.border.outside.color" ) ) ) );
			draw->AddRectFilled( pos + ImVec2( 5, 3 ), pos + size - ImVec2( 5, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.armor.border.inside.color" ) ) ) );

			draw->AddRectFilled( pos + ImVec2( 5, 3 ), pos + size - ImVec2( 56, 3 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.armor.color" ) ) ) );
		}

		if ( ImGui::BeginPopup( "Armor" ) ) {
			elements::child( "Armor Settings", { 200, 135 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.team.armor.color" ), NULL );
				elements::color_edit4( "Color Outline 1", FNV1A( "esp.team.armor.border.outside.color" ), NULL );
				elements::color_edit4( "Color Outline 2", FNV1A( "esp.team.armor.border.inside.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Armor" );
		}
	}

	inline void BoxTeammates( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond )
	{
		if ( cond == CENTER_COND ) {
			size = ImVec2( 110, 230 );

			draw->AddRect( pos + ImVec2( 4, 0 ), pos + size - ImVec2( 4, 0 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.box.color" ) ) ) );
			draw->AddRect( pos + ImVec2( 3, -1 ), pos + size - ImVec2( 3, -1 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.box.border.outside.color" ) ) ) );
			draw->AddRect( pos + ImVec2( 5, 1 ), pos + size - ImVec2( 5, 1 ), toColor( cfg::get<col_t>( FNV1A( "esp.team.box.border.inside.color" ) ) ) );
		}

		if ( ImGui::BeginPopup( "Box" ) ) {
			elements::child( "Box Settings", { 200, 135 }, [ ] ( ) {
				elements::color_edit4( "Color", FNV1A( "esp.team.box.color" ), NULL );
				elements::color_edit4( "Color Outline 1", FNV1A( "esp.team.box.border.outside.color" ), NULL );
				elements::color_edit4( "Color Outline 2", FNV1A( "esp.team.box.border.inside.color" ), NULL );
			} );
			ImGui::EndPopup( );
		}

		if ( pressed ) {
			ImGui::OpenPopup( "Box" );
		}
	}

	inline void GetDrawableTeammates( bool pressed, ImDrawList* draw, ImVec2 pos, ImVec2& size, int cond, int var ) {
		switch ( var ) {
		case 0: return WeaponTeammates( pressed, draw, pos, size, cond ); break;
		case 1: return UsernameTeammates( pressed, draw, pos, size, cond ); break;
		case 2: return HealthBarTeammates( pressed, draw, pos, size, cond ); break;
		case 3: return ArmorBarTeammates( pressed, draw, pos, size, cond ); break;
		case 4: return BoxTeammates( pressed, draw, pos, size, cond ); break;
		}
	}
}