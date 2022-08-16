#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>

#include "../utils/render/render.h"

#include "../utils/imgui/imgui.h"
#include "../utils/imgui/imgui_internal.h"

namespace notifies 
{
	struct NotifyStruct {
		const char* message;
		const char* second;
		ULONGLONG time;
	};

	inline std::vector<NotifyStruct> notifies;

	inline void push( const char* message, const char* second ) {
		notifies.push_back( { message, second, GetTickCount64( ) + 3000 } );
	}

	inline void handle( ImDrawList* draw ) {

		if ( notifies.empty( ) )
			return;

		const auto ScreenSize = ImGui::GetIO( ).DisplaySize;
		ImVec2 NotifyPos = ScreenSize - ImVec2( 320.f, ScreenSize.y - 10 );

		auto DrawNotify = [ &draw, &NotifyPos ] ( NotifyStruct notify ) {

			float alpha = 1.f;
			if ( notify.time - 500 <= GetTickCount64( ) )
			{
				const auto delta = std::clamp<float>( notify.time - GetTickCount64( ), 0.f, 500 );
				alpha = std::clamp<float>( delta / 500, 0.f, 1.f );
			}

			if ( alpha == 0 )
				return;

			const auto NotifyEndPos = NotifyPos + ImVec2( 300, 42 );

			draw->AddRectFilled( NotifyPos, NotifyEndPos, ImColor( 34, 34, 34, int(255 * alpha ) ), 4 );
			draw->AddRect( NotifyPos + ImVec2(2, 2), NotifyEndPos - ImVec2(2, 2), ImColor( 87, 87, 87, int( 255 * alpha ) ), 4, 15, 4 );
			draw->AddRect( NotifyPos, NotifyEndPos, ImColor( 1, 1, 1, int( 255 * alpha ) ), 4, 15 );

			draw->AddImage( assets::icons[5], NotifyPos + ImVec2(13, 13), NotifyPos + ImVec2( 29, 29 ), {}, {1, 1}, ImColor(255, 255, 255, int( 255 * alpha ) ) );

			const auto TextSize = ImGui::CalcTextSize( notify.message );
			const auto TextPos = NotifyPos + ImVec2( 15.f, 10 );

			draw->AddText( NotifyPos + ImVec2( 37, 7 ), ImColor( 251, 253, 108, int( 255 * alpha ) ), notify.message );
			draw->AddText( NotifyPos + ImVec2( 37, 19 ), ImColor( 254, 254, 254, int( 255 * alpha ) ), notify.second );

			NotifyPos.y += 45.f;
		};

		for ( int i = 0; i < notifies.size( ); i++ )
		{
			auto current_notify = notifies.at( i );

			if ( current_notify.time < GetTickCount64( ) ) {
				notifies.erase( notifies.begin( ) + i );
				continue;
			}

			DrawNotify( current_notify );
		}
	}
}