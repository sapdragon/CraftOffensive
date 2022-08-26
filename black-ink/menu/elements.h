#pragma once

#include "../utils/render/render.h"
#include "../api/api.h"

#include "../utils/imgui/imgui.h"
#include "../utils/imgui/imgui_internal.h"

#include "../utils/imgui/imgui_tweaker.hpp"

#include <functional>

namespace elements {
	inline ImAnimations::Animator<std::string, float> animationsPressed;
	inline ImAnimations::Animator<std::string, float> animationsHovered;

	static const char* __PatchFormatStringFloatToInt( const char* fmt )
	{
		if ( fmt[ 0 ] == '%' && fmt[ 1 ] == '.' && fmt[ 2 ] == '0' && fmt[ 3 ] == 'f' && fmt[ 4 ] == 0 ) // Fast legacy path for "%.0f" which is expected to be the most common case.
			return "%d";
		const char* fmt_start = ImParseFormatFindStart( fmt );    // Find % (if any, and ignore %%)
		const char* fmt_end = ImParseFormatFindEnd( fmt_start );  // Find end of format specifier, which itself is an exercise of confidence/recklessness (because snprintf is dependent on libc or user).
		if ( fmt_end > fmt_start && fmt_end[ -1 ] == 'f' )
		{
			if ( fmt_start == fmt && fmt_end[ 0 ] == 0 )
				return "%d";
			ImGuiContext& g = *GImGui;
			ImFormatString( g.TempBuffer, IM_ARRAYSIZE( g.TempBuffer ), "%.*s%%d%s", ( int ) ( fmt_start - fmt ), fmt, fmt_end ); // Honor leading and trailing decorations, but lose alignment/precision.
			return g.TempBuffer;
		}
		return fmt;
	}

	inline void tab( std::string label, ImTextureID texture, int& selected, int index ) {
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow( );
		if ( window->SkipItems )
			return;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID( label.c_str( ) );
		const ImVec2 label_size = CalcTextSize( label.c_str( ), NULL, true );

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = CalcItemSize( { 110, 50 }, 100, 50 );

		const ImRect bb( pos, pos + size );
		ItemSize( size, style.FramePadding.y );
		if ( !ItemAdd( bb, id ) )
			return;

		bool hovered, held;
		bool pressed = ButtonBehavior( bb, id, &hovered, &held, NULL );

		if ( pressed ) selected = index;

		auto hoveredAnimate = animationsHovered.ValueInOutElastic( label, hovered, 0.f, 1.f, 0.05f );
		auto selectedAnimate = animationsPressed.ValueOutQuart( label, selected == index, 0.f, 1.f, 0.05f );

		ImColor selectedRect = ImAnimations::LerpColor( ImColor( 51, 51, 51, 0 ), ImColor( 51, 51, 51 ), selectedAnimate );

		window->DrawList->AddRectFilled( bb.Min, bb.Max, selectedRect );
		window->DrawList->AddImage( texture, bb.Min + ImVec2( 47, 8 ), bb.Min + ImVec2( 63, 24 ) );
		window->DrawList->AddText( bb.Min + ImVec2( 55 - label_size.x / 2, 30 ), ImColor( 220, 220, 220 ), label.c_str( ) );
	}

	inline void subtab( std::string label, ImVec2 size_arg, int& selected, int index ) {
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow( );
		if ( window->SkipItems )
			return;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID( label.c_str( ) );
		const ImVec2 label_size = CalcTextSize( label.c_str( ), NULL, true );

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = CalcItemSize( size_arg, 100, 50 );

		const ImRect bb( pos, pos + size );
		ItemSize( size, style.FramePadding.y );
		if ( !ItemAdd( bb, id ) )
			return;

		bool hovered, held;
		bool pressed = ButtonBehavior( bb, id, &hovered, &held, NULL );

		if ( pressed ) selected = index;

		auto hoveredAnimate = animationsHovered.ValueInOutElastic( label, hovered, 0.f, 1.f, 0.05f );
		auto selectedAnimate = animationsPressed.ValueOutQuart( label, selected == index, 0.f, 1.f, 0.05f );

		ImColor selectedRect = ImAnimations::LerpColor( ImColor( 44, 44, 44, 0 ), ImColor( 44, 44, 44 ), selectedAnimate );

		window->DrawList->AddRectFilled( bb.Min, bb.Max, selectedRect );
		window->DrawList->AddText( bb.Min + ImVec2( size_arg.x / 2 - label_size.x / 2, 8 ), ImColor( 220, 220, 220 ), label.c_str( ) );
	}

	inline void child( std::string label, ImVec2 size_arg, std::function<void( )> func ) {
		ImGui::BeginChild( label.c_str( ), size_arg, false, ImGuiWindowFlags_NoBackground );
		{
			auto pos = ImGui::GetWindowPos( );

			ImGui::GetWindowDrawList( )->AddRectFilled( pos, pos + size_arg, ImColor( 35, 35, 35, 220 ) );

			ImGui::GetWindowDrawList( )->PushClipRect( pos, pos + ImVec2( size_arg.x, 30 ), false );
			ImGui::GetWindowDrawList( )->AddImage( assets::dirt, pos, pos + ImVec2{ 600, 51 } );
			ImGui::GetWindowDrawList( )->AddRectFilled( pos, pos + ImVec2( size_arg.x, 30 ), ImColor( 33, 33, 33, 110 ) );
			ImGui::GetWindowDrawList( )->PopClipRect( );

			ImGui::GetWindowDrawList( )->AddText( pos + ImVec2( 10, 7 ), ImColor( 220, 220, 220 ), label.c_str( ) );

			ImGui::SetCursorPos( { 10, 39 } );
			ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 10, 6 } );
			ImGui::BeginChild( std::string( label + "child" ).c_str( ), size_arg - ImVec2( 20, 44 ), false, ImGuiWindowFlags_NoBackground );
			{
				func( );
			}
			ImGui::EndChild( );
			ImGui::PopStyleVar( );
		}
		ImGui::EndChild( );
	}

	inline void child_internal( std::string label, ImVec2 size_arg ) {
		ImGui::BeginChild( label.c_str( ), size_arg, false, ImGuiWindowFlags_NoBackground );

		auto pos = ImGui::GetWindowPos( );

		ImGui::GetWindowDrawList( )->AddRectFilled( pos, pos + size_arg, ImColor( 35, 35, 35, 220 ) );

		ImGui::GetWindowDrawList( )->PushClipRect( pos, pos + ImVec2( size_arg.x, 30 ), false );
		ImGui::GetWindowDrawList( )->AddImage( assets::dirt, pos, pos + ImVec2{ 600, 51 } );
		ImGui::GetWindowDrawList( )->AddRectFilled( pos, pos + ImVec2( size_arg.x, 30 ), ImColor( 33, 33, 33, 110 ) );
		ImGui::GetWindowDrawList( )->PopClipRect( );

		ImGui::GetWindowDrawList( )->AddText( pos + ImVec2( 10, 7 ), ImColor( 220, 220, 220 ), label.c_str( ) );

		ImGui::SetCursorPos( { 10, 39 } );
		ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, { 10, 6 } );
		ImGui::BeginChild( std::string( label + "child" ).c_str( ), size_arg - ImVec2( 20, 44 ), false, ImGuiWindowFlags_NoBackground );
	}

	inline void child_end_internal( ) {
		ImGui::EndChild( );
		ImGui::PopStyleVar( );
		ImGui::EndChild( );
	}


	inline void checkbox( std::string label, uint32_t hash ) {
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow( );
		if ( window->SkipItems )
			return;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID( label.c_str( ) );
		const ImVec2 label_size = CalcTextSize( label.c_str( ), NULL, true );

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = CalcItemSize( { ImGui::GetWindowSize( ).x, 22 }, 100, 50 );

		const ImRect bb( pos, pos + size );
		ItemSize( size, style.FramePadding.y );
		if ( !ItemAdd( bb, id ) )
			return;

		bool hovered, held;
		bool pressed = ButtonBehavior( bb, id, &hovered, &held, NULL );

		if ( pressed )
			cfg::set<bool>( hash, !cfg::get<bool>( hash ) );

		auto hoveredAnimate = animationsHovered.ValueInOutElastic( label, hovered, 0.f, 1.f, 0.05f );
		auto selectedAnimate = animationsPressed.ValueOutQuart( label, cfg::get<bool>( hash ), 0.f, 1.f, 0.05f );

		ImColor selectedCheck = ImAnimations::LerpColor( ImColor( 151, 151, 151, 0 ), ImColor( 151, 151, 151 ), selectedAnimate );

		window->DrawList->AddRectFilled( bb.Min + ImVec2( 0, 2 ), bb.Min + ImVec2( 20, 22 ), ImColor( 28, 28, 27 ) );

		window->DrawList->PushClipRect( bb.Min + ImVec2( 0, 2 ), bb.Min + ImVec2( 20, 22 ), false );
		window->DrawList->AddImage( assets::dirt, bb.Min + ImVec2( 0, 2 ), bb.Min + ImVec2( 20, 22 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		window->DrawList->PopClipRect( );

		window->DrawList->AddImage( assets::diamond, bb.Min + ImVec2( 1, 3 ), bb.Min + ImVec2{ 19, 21 }, {}, { 1, 1 }, ImColor( 255, 255, 255, int( 255 * selectedAnimate ) ) );

		window->DrawList->AddRect( bb.Min + ImVec2( 0, 2 ), bb.Min + ImVec2( 20, 22 ), ImColor( 0, 0, 0 ) );

		window->DrawList->AddText( bb.Min + ImVec2( 27, 5 ), ImColor( 220, 220, 220 ), label.c_str( ) );
	}

	inline void checkbox_bool( std::string label, bool& v ) {
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow( );
		if ( window->SkipItems )
			return;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID( label.c_str( ) );
		const ImVec2 label_size = CalcTextSize( label.c_str( ), NULL, true );

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = CalcItemSize( { ImGui::GetWindowSize( ).x, 22 }, 100, 50 );

		const ImRect bb( pos, pos + size );
		ItemSize( size, style.FramePadding.y );
		if ( !ItemAdd( bb, id ) )
			return;

		bool hovered, held;
		bool pressed = ButtonBehavior( bb, id, &hovered, &held, NULL );

		if ( pressed )
			v = !v;

		auto hoveredAnimate = animationsHovered.ValueInOutElastic( label, hovered, 0.f, 1.f, 0.05f );
		auto selectedAnimate = animationsPressed.ValueOutQuart( label, v, 0.f, 1.f, 0.05f );

		ImColor selectedCheck = ImAnimations::LerpColor( ImColor( 151, 151, 151, 0 ), ImColor( 151, 151, 151 ), selectedAnimate );

		window->DrawList->AddRectFilled( bb.Min + ImVec2( 0, 2 ), bb.Min + ImVec2( 20, 22 ), ImColor( 28, 28, 27 ) );

		window->DrawList->PushClipRect( bb.Min + ImVec2( 0, 2 ), bb.Min + ImVec2( 20, 22 ), false );
		window->DrawList->AddImage( assets::dirt, bb.Min + ImVec2( 0, 2 ), bb.Min + ImVec2( 20, 22 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		window->DrawList->PopClipRect( );

		window->DrawList->AddImage( assets::diamond, bb.Min + ImVec2( 1, 3 ), bb.Min + ImVec2{ 19, 21 }, {}, { 1, 1 }, ImColor( 255, 255, 255, int( 255 * selectedAnimate ) ) );

		window->DrawList->AddRect( bb.Min + ImVec2( 0, 2 ), bb.Min + ImVec2( 20, 22 ), ImColor( 0, 0, 0 ) );

		window->DrawList->AddText( bb.Min + ImVec2( 27, 5 ), ImColor( 220, 220, 220 ), label.c_str( ) );
	}

	inline int __DataTypeFormatString( char* buf, int buf_size, ImGuiDataType data_type, const void* p_data, const char* format )
	{
		// Signedness doesn't matter when pushing integer arguments
		if ( data_type == ImGuiDataType_S32 || data_type == ImGuiDataType_U32 )
			return ImFormatString( buf, buf_size, format, *( const ImU32* ) p_data );
		if ( data_type == ImGuiDataType_S64 || data_type == ImGuiDataType_U64 )
			return ImFormatString( buf, buf_size, format, *( const ImU64* ) p_data );
		if ( data_type == ImGuiDataType_Float )
			return ImFormatString( buf, buf_size, format, *( const float* ) p_data );
		if ( data_type == ImGuiDataType_Double )
			return ImFormatString( buf, buf_size, format, *( const double* ) p_data );
		if ( data_type == ImGuiDataType_S8 )
			return ImFormatString( buf, buf_size, format, *( const ImS8* ) p_data );
		if ( data_type == ImGuiDataType_U8 )
			return ImFormatString( buf, buf_size, format, *( const ImU8* ) p_data );
		if ( data_type == ImGuiDataType_S16 )
			return ImFormatString( buf, buf_size, format, *( const ImS16* ) p_data );
		if ( data_type == ImGuiDataType_U16 )
			return ImFormatString( buf, buf_size, format, *( const ImU16* ) p_data );
		IM_ASSERT( 0 );
		return 0;
	}

	inline bool __sliderscalar( const char* label, ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max, const char* format, float power )
	{
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow( );
		if ( window->SkipItems )
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID( label );
		const float w = CalcItemWidth( );

		const ImVec2 label_size = CalcTextSize( label, NULL, true );
		const ImRect frame_bb( window->DC.CursorPos, window->DC.CursorPos + ImVec2( ImGui::GetWindowSize( ).x, 25 ) );
		const ImRect total_bb( window->DC.CursorPos, window->DC.CursorPos + ImVec2( ImGui::GetWindowSize( ).x, 25 ) );

		ItemSize( total_bb, style.FramePadding.y );
		if ( !ItemAdd( total_bb, id, &frame_bb ) )
			return false;

		if ( format == NULL )
			format = DataTypeGetInfo( data_type )->PrintFmt;
		else if ( data_type == ImGuiDataType_S32 && strcmp( format, "%d" ) != 0 )
			format = __PatchFormatStringFloatToInt( format );

		const bool hovered = ItemHoverable( frame_bb, id );

		const bool focus_requested = FocusableItemRegister( window, id );
		const bool clicked = ( hovered && g.IO.MouseClicked[ 0 ] );
		if ( focus_requested || clicked || g.NavActivateId == id || g.NavInputId == id )
		{
			SetActiveID( id, window );
			SetFocusID( id, window );
			FocusWindow( window );
			g.ActiveIdUsingNavDirMask |= ( 1 << ImGuiDir_Left ) | ( 1 << ImGuiDir_Right );

			if ( focus_requested || ( clicked && g.IO.KeyCtrl ) || g.NavInputId == id )
			{
				FocusableItemUnregister( window );
			}
		}

		window->DrawList->PushClipRect( frame_bb.Min, frame_bb.Max, false );
		window->DrawList->AddImage( assets::background, frame_bb.Min, frame_bb.Min + ImVec2( 600, 445 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		window->DrawList->PopClipRect( );

		window->DrawList->AddRectFilled( frame_bb.Min, frame_bb.Max, ImColor( 47, 47, 47, 90 ), 0, 0 );

		auto hoveredAnimate = animationsHovered.ValueInSine( label, hovered, 0.f, 1.f, 0.05f );
		window->DrawList->AddRectFilled( frame_bb.Min, frame_bb.Max, ImColor( 120, 120, 120, int( 50 * hoveredAnimate ) ) );


		ImRect grab_bb;
		const bool value_changed = SliderBehavior( frame_bb, id, data_type, p_data, p_min, p_max, format, power, ImGuiSliderFlags_None, &grab_bb );

		if ( grab_bb.Max.x > grab_bb.Min.x )
		{
			window->DrawList->AddRectFilled( grab_bb.Min - ImVec2( 1, 1 ), grab_bb.Max + ImVec2( 1, 1 ), ImColor( 170, 169, 167 ) );
			window->DrawList->AddRectFilled( grab_bb.Min + ImVec2( 0, 0 ), grab_bb.Max + ImVec2( 1, 1 ), ImColor( 85, 85, 85 ) );
			window->DrawList->AddRectFilled( grab_bb.Min, grab_bb.Max - ImVec2( 1, 1 ), ImColor( 110, 109, 107 ) );

			window->DrawList->AddRect( grab_bb.Min - ImVec2( 2, 2 ), grab_bb.Max + ImVec2( 2, 2 ), ImColor( 0, 0, 0 ) );
		}

		window->DrawList->AddRect( frame_bb.Min, frame_bb.Max, ImColor( 0, 0, 0 ), 0, 0 );

		char value_buf[ 64 ];
		const char* value_buf_end = value_buf + DataTypeFormatString( value_buf, IM_ARRAYSIZE( value_buf ), data_type, p_data, format );

		RenderTextClipped( frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2( 0.5f, 0.5f ) );

		return value_changed;
	}

	inline void slider_floats( std::string label, float& v, float v_min, float v_max, const char* format = "%0.1f", float power = 1.f )
	{
		__sliderscalar( label.c_str( ), ImGuiDataType_Float, &v, &v_min, &v_max, format, 1 );
	}

	inline void slider_ints( std::string label, int& v, int v_min, int v_max, const char* format = "%i" )
	{
		__sliderscalar( label.c_str( ), ImGuiDataType_S32, &v, &v_min, &v_max, format, 1 );
	}

	inline void slider_float( std::string label, uint32_t v, float v_min, float v_max, const char* format = "%0.1f", float power = 1.f )
	{
		float value = cfg::get<float>( v );

		if ( __sliderscalar( label.c_str( ), ImGuiDataType_Float, &value, &v_min, &v_max, format, 1 ) )
			cfg::set<float>( v, value );
	}

	inline void slider_int( std::string label, uint32_t v, int v_min, int v_max, const char* format = "%i" )
	{
		int value = cfg::get<int>( v );

		if ( __sliderscalar( label.c_str( ), ImGuiDataType_S32, &value, &v_min, &v_max, format, 1 ) )
			cfg::set<int>( v, value );
	}

	inline bool button( const char* label, ImVec2 size_arg ) {
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow( );
		if ( window->SkipItems )
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID( label );
		const ImVec2 label_size = CalcTextSize( label, NULL, true );

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = CalcItemSize( size_arg, 100, 50 );

		const ImRect bb( pos, pos + size );
		ItemSize( size, style.FramePadding.y );
		if ( !ItemAdd( bb, id ) )
			return false;

		bool hovered, held;
		bool pressed = ButtonBehavior( bb, id, &hovered, &held, NULL );

		window->DrawList->PushClipRect( bb.Min, bb.Max, false );
		window->DrawList->AddImage( assets::background, bb.Min, bb.Min + ImVec2( 600, 445 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		window->DrawList->PopClipRect( );

		auto hoveredAnimate = animationsHovered.ValueInSine( label, hovered, 0.f, 1.f, 0.05f );
		window->DrawList->AddRectFilled( bb.Min, bb.Max, ImColor( 120, 120, 120, int( 50 * hoveredAnimate ) ) );

		RenderTextClipped( bb.Min, bb.Max, label, "", NULL, ImVec2( 0.5f, 0.5f ) );

		window->DrawList->AddRect( bb.Min, bb.Max, ImColor( 0, 0, 0 ) );

		return pressed;
	}

	inline bool config( config_t conf ) {
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow( );
		if ( window->SkipItems )
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID( conf.m_secure_id.c_str( ) );

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = CalcItemSize( { 405, 50 }, 100, 50 );

		const ImRect bb( pos, pos + size );
		ItemSize( size, style.FramePadding.y );
		if ( !ItemAdd( bb, id ) )
			return false;

		bool hovered, held;
		bool pressed = ButtonBehavior( bb, id, &hovered, &held, NULL );

		window->DrawList->PushClipRect( bb.Min, bb.Max, true );
		window->DrawList->AddImage( assets::dirt, bb.Min, bb.Min + ImVec2( 600, 51 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		window->DrawList->PopClipRect( );

		auto hoveredAnimate = animationsHovered.ValueInSine( conf.m_secure_id, hovered, 0.f, 1.f, 0.05f );
		window->DrawList->AddRectFilled( bb.Min, bb.Max, ImColor( 45, 45, 45, int( 50 * hoveredAnimate ) ) );

		window->DrawList->AddImage( assets::creeper, bb.Min + ImVec2( 4, 4 ), bb.Min + ImVec2( 46, 46 ) );
		window->DrawList->AddRect( bb.Min + ImVec2( 4, 4 ), bb.Min + ImVec2( 46, 46 ), ImColor( 0, 0, 0 ) );

		window->DrawList->AddText( bb.Min + ImVec2( 52, 8 ), ImColor( 255, 255, 255 ), conf.m_name.c_str( ) );
		window->DrawList->AddText( bb.Min + ImVec2( 52, 26 ), ImColor( 190, 190, 190 ), std::string( "By " + conf.m_author ).c_str( ) );

		window->DrawList->AddRect( bb.Min, bb.Max, ImColor( 0, 0, 0 ) );

		window->DrawList->PushClipRect( bb.Min + ImVec2{ 405 - 10 - 50, 10 }, bb.Min + ImVec2{ 405 - 10, 40 }, true );
		window->DrawList->AddImage( assets::background, bb.Min, bb.Min + ImVec2( 600, 445 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		window->DrawList->AddText( bb.Min + ImVec2{ 405 - 10 - 25, 24 } - ImGui::CalcTextSize( "Load" ) / 2, ImColor( 255, 255, 255 ), "Load" );
		window->DrawList->AddRect( bb.Min + ImVec2{ 405 - 10 - 50, 10 }, bb.Min + ImVec2{ 405 - 10, 40 }, ImColor( 0, 0, 0 ) );
		window->DrawList->PopClipRect( );

		if ( ImGui::IsMouseHoveringRect( bb.Min + ImVec2{ 405 - 10 - 50, 10 }, bb.Min + ImVec2{ 405 - 10, 40 } ) && ImGui::IsMouseClicked( 0 ) ) {
			cloud->load_config( conf.m_secure_id );
		}

		window->DrawList->PushClipRect( bb.Min + ImVec2{ 405 - 20 - 100, 10 }, bb.Min + ImVec2{ 405 - 20 - 50, 40 }, true );
		window->DrawList->AddImage( assets::background, bb.Min, bb.Min + ImVec2( 600, 445 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		window->DrawList->AddText( bb.Min + ImVec2{ 405 - 20 - 75, 24 } - ImGui::CalcTextSize( "Save" ) / 2, ImColor( 255, 255, 255 ), "Save" );
		window->DrawList->AddRect( bb.Min + ImVec2{ 405 - 20 - 100, 10 }, bb.Min + ImVec2{ 405 - 20 - 50, 40 }, ImColor( 0, 0, 0 ) );
		window->DrawList->PopClipRect( );

		if ( ImGui::IsMouseHoveringRect( bb.Min + ImVec2{ 405 - 20 - 100, 10 }, bb.Min + ImVec2{ 405 - 20 - 50, 40 } ) && ImGui::IsMouseClicked( 0 ) ) {
			cloud->save_config( conf.m_secure_id );
		}

		window->DrawList->PushClipRect( bb.Min + ImVec2{ 405 - 30 - 150, 10 }, bb.Min + ImVec2{ 405 - 30 - 100, 40 }, true );
		window->DrawList->AddImage( assets::background, bb.Min, bb.Min + ImVec2( 600, 445 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		window->DrawList->AddText( bb.Min + ImVec2{ 405 - 30 - 125, 24 } - ImGui::CalcTextSize( "Delete" ) / 2, ImColor( 255, 255, 255 ), "Delete" );
		window->DrawList->AddRect( bb.Min + ImVec2{ 405 - 30 - 150, 10 }, bb.Min + ImVec2{ 405 - 30 - 100, 40 }, ImColor( 0, 0, 0 ) );
		window->DrawList->PopClipRect( );

		if ( ImGui::IsMouseHoveringRect( bb.Min + ImVec2{ 405 - 30 - 150, 10 }, bb.Min + ImVec2{ 405 - 30 - 100, 40 } ) && ImGui::IsMouseClicked( 0 ) ) {
			cloud->delete_config( conf.m_secure_id );
		}

		return pressed;
	}

	inline bool color_edit4( const char* label, col_t* v, ImGuiColorEditFlags flags )
	{
		auto clr = ImVec4{
			v->r( ) / 255.0f,
			v->g( ) / 255.0f,
			v->b( ) / 255.0f,
			v->a( ) / 255.0f
		};

		if ( ImGui::ColorEdit4( label, &clr.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoOptions | flags ) ) {
			*v = col_t( clr.x * 255, clr.y * 255, clr.z * 255, clr.w * 255 );
			return true;
		}
		return false;
	}

	inline bool color_edit3( const char* label, col_t* v )
	{
		return color_edit4( label, v, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoOptions );
	}

	inline bool color_edit4( const char* label, uint32_t v, ImGuiColorEditFlags flags )
	{
		auto clr = ImVec4{
			cfg::get<col_t>( v ).r( ) / 255.0f,
			cfg::get<col_t>( v ).g( ) / 255.0f,
			cfg::get<col_t>( v ).b( ) / 255.0f,
			cfg::get<col_t>( v ).a( ) / 255.0f
		};

		if ( ImGui::ColorEdit4( label, &clr.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoOptions | flags ) ) {
			cfg::set<col_t>( v, col_t( clr.x * 255, clr.y * 255, clr.z * 255, clr.w * 255 ) );
			return true;
		}
		return false;
	}

	inline bool color_edit3( const char* label, uint32_t v )
	{
		return color_edit4( label, v, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoOptions );
	}

	inline bool chams_item( int index, std::vector<chams_material_settings_t>& source ) {
		using namespace ImGui;

		ImGuiWindow* window = GetCurrentWindow( );
		if ( window->SkipItems )
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID( std::string( std::to_string( index ) + source[ index ].label ).c_str( ) );

		ImVec2 pos = window->DC.CursorPos;
		ImVec2 size = CalcItemSize( { ImGui::GetWindowSize( ).x - 5, 26 }, 100, 50 );

		const ImRect bb( pos, pos + size );
		ItemSize( size, style.FramePadding.y );
		if ( !ItemAdd( bb, id ) )
			return false;

		bool hovered, held;
		bool pressed = ButtonBehavior( bb, id, &hovered, &held, NULL );

		window->DrawList->PushClipRect( bb.Min, bb.Max, true );
		window->DrawList->AddImage( assets::background, bb.Min, bb.Min + ImVec2( 600, 445 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		window->DrawList->PopClipRect( );

		auto hoveredAnimate = animationsHovered.ValueInSine( std::string( std::to_string( index ) + source[ index ].label ), hovered, 0.f, 1.f, 0.05f );
		window->DrawList->AddRectFilled( bb.Min, bb.Max, ImColor( 120, 120, 120, int( 50 * hoveredAnimate ) ) );
		window->DrawList->AddRect( bb.Min, bb.Max, ImColor( 0, 0, 0 ) );

		// enabled
		window->DrawList->AddRectFilled( bb.Min + ImVec2( 3, 3 ), bb.Min + ImVec2( 23, 23 ), ImColor( 28, 28, 27 ) );

		window->DrawList->PushClipRect( bb.Min + ImVec2( 3, 3 ), bb.Min + ImVec2( 23, 23 ), true );
		window->DrawList->AddImage( assets::dirt, bb.Min + ImVec2( 3, 3 ), bb.Min + ImVec2( 23, 23 ), {}, { 1, 1 }, ImColor( 255, 255, 255, int( 150 ) ) );
		if ( source[ index ].m_enable ) {
			window->DrawList->AddImage( assets::diamond, bb.Min + ImVec2( 3, 3 ), bb.Min + ImVec2( 23, 23 ), {}, { 1, 1 }, ImColor( 255, 255, 255, 255 ) );
		}
		window->DrawList->AddRect( bb.Min + ImVec2( 3, 3 ), bb.Min + ImVec2( 23, 23 ), ImColor( 0, 0, 0 ) );
		window->DrawList->PopClipRect( );


		// label
		window->DrawList->AddText( bb.Min + ImVec2( 30, 5 ), ImColor( 255, 255, 255 ), source[ index ].label.c_str( ) );

		if ( ImGui::IsMouseHoveringRect( bb.Min + ImVec2( 3, 3 ), bb.Min + ImVec2( 23, 23 ) ) && ImGui::IsMouseClicked( 0 ) )
			source[ index ].m_enable = !source[ index ].m_enable;

		if ( source.size( ) != 1 ) {
			if ( index != source.size( ) - 1 ) {
				window->DrawList->AddText( bb.Max - ImVec2( 13, 13 ) - ImGui::CalcTextSize( "D" ) / 2, ImColor( 255, 255, 255 ), "D" );
				if ( ImGui::IsMouseHoveringRect( bb.Max - ImVec2( 26, 26 ), bb.Max ) && ImGui::IsMouseClicked( 0 ) )
					std::swap( source[ index ], source[ index + 1 ] );
			}
			if ( index != 0 ) {
				window->DrawList->AddText( bb.Max - ImVec2( 13 + 26, 13 ) - ImGui::CalcTextSize( "U" ) / 2, ImColor( 255, 255, 255 ), "U" );
				if ( ImGui::IsMouseHoveringRect( bb.Max - ImVec2( 52, 26 ), bb.Max - ImVec2( 26, 0 ) ) && ImGui::IsMouseClicked( 0 ) )
					std::swap( source[ index ], source[ index - 1 ] );
			}
		}

		return pressed;
	}

	inline void combo( const char* label, uint32_t hash, const char* const items[], int items_count )
	{
		int val = cfg::get<int>( hash );
		if ( ImGui::Combo( label, &val, items, items_count ) )
			cfg::set<int>( hash, val );
	}
}