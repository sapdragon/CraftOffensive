#pragma once

#include "../utils/render/render.h"

#include "../utils/imgui/imgui.h"
#include "../utils/imgui/imgui_internal.h"

#include "../utils/imgui/imgui_tweaker.hpp"

#include <functional>

namespace elements {
	inline ImAnimations::Animator<std::string, float> animationsPressed;
    inline ImAnimations::Animator<std::string, float> animationsHovered;

	inline void tab( std::string label, ImTextureID texture, int& selected, int index ) {
        using namespace ImGui;

        ImGuiWindow* window = GetCurrentWindow( );
        if ( window->SkipItems )
            return;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID( label.c_str() );
        const ImVec2 label_size = CalcTextSize( label.c_str(), NULL, true );

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = CalcItemSize( {110, 50}, 100, 50 );

        const ImRect bb( pos, pos + size );
        ItemSize( size, style.FramePadding.y );
        if ( !ItemAdd( bb, id ) )
            return;

        bool hovered, held;
        bool pressed = ButtonBehavior( bb, id, &hovered, &held, NULL );

        if ( pressed ) selected = index;

        auto hoveredAnimate = animationsHovered.ValueInOutElastic( label, hovered, 0.f, 1.f, 0.05f );
        auto selectedAnimate = animationsPressed.ValueOutQuart( label, selected == index, 0.f, 1.f, 0.05f );

        ImColor selectedRect = ImAnimations::LerpColor( ImColor( 51, 51, 51, 0 ), ImColor( 51, 51, 51 ), selectedAnimate);

        window->DrawList->AddRectFilled( bb.Min, bb.Max, selectedRect );
        window->DrawList->AddImage( texture, bb.Min + ImVec2( 47,  8), bb.Min + ImVec2(63, 24) );
        window->DrawList->AddText( bb.Min + ImVec2(55 - label_size.x / 2, 30), ImColor(220, 220, 220 ), label.c_str());
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
        window->DrawList->AddText( bb.Min + ImVec2( size_arg.x / 2 - label_size.x/2, 8 ), ImColor( 220, 220, 220 ), label.c_str( ) );
    }

    inline void child( std::string label, ImVec2 size_arg, std::function<void( )> func ) {
        ImGui::BeginChild( label.c_str(), size_arg, false, ImGuiWindowFlags_NoBackground );
        {
            auto pos = ImGui::GetWindowPos( );

            ImGui::GetWindowDrawList( )->AddRectFilled( pos, pos + size_arg, ImColor( 44, 44, 44, 220 ) );

            ImGui::GetWindowDrawList( )->PushClipRect( pos, pos + ImVec2( size_arg.x, 30 ), false );
            ImGui::GetWindowDrawList( )->AddImage( assets::dirt, pos, pos + ImVec2{600, 51} );
            ImGui::GetWindowDrawList( )->AddRectFilled( pos, pos + ImVec2( size_arg.x, 30 ), ImColor( 33, 33, 33, 110 ) );
            ImGui::GetWindowDrawList( )->PopClipRect( );

            ImGui::GetWindowDrawList( )->AddText( pos + ImVec2(10, 7), ImColor( 220, 220, 220 ), label.c_str( ) );

            ImGui::SetCursorPos( {10, 35} );
            ImGui::BeginChild( std::string(label+"child").c_str(), size_arg - ImVec2( 20, 40 ), false, ImGuiWindowFlags_NoBackground );
            {
               func( );
            }
            ImGui::EndChild( );
        }
        ImGui::EndChild( );
    }

    inline void checkbox( std::string label, uint32_t hash) {
        using namespace ImGui;

        ImGuiWindow* window = GetCurrentWindow( );
        if ( window->SkipItems )
            return;

        ImGuiContext& g = *GImGui;
        const ImGuiStyle& style = g.Style;
        const ImGuiID id = window->GetID( label.c_str( ) );
        const ImVec2 label_size = CalcTextSize( label.c_str( ), NULL, true );

        ImVec2 pos = window->DC.CursorPos;
        ImVec2 size = CalcItemSize( { ImGui::GetWindowSize().x, 25}, 100, 50 );

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

        window->DrawList->AddRectFilled( bb.Min + ImVec2(0, 4), bb.Min + ImVec2(17, 21), ImColor( 28, 28, 27 ) );

        window->DrawList->PushClipRect( bb.Min + ImVec2( 0, 4 ), bb.Min + ImVec2( 17, 21 ), false );
        window->DrawList->AddImage( assets::dirt, bb.Min + ImVec2( 0, 4 ), bb.Min + ImVec2( 17, 21 ), {}, {1, 1}, ImColor(255, 255, 255, int(150 )) );
        window->DrawList->PopClipRect( );

        window->DrawList->AddImage( assets::diamond, bb.Min + ImVec2( 1, 5 ), bb.Min + ImVec2{ 16, 19 }, {}, { 1, 1 }, ImColor( 255, 255, 255, int( 255 * selectedAnimate ) ) );

        window->DrawList->AddRect( bb.Min + ImVec2( 0, 4 ), bb.Min + ImVec2( 17, 21 ), ImColor( 194, 194, 194 ) );

        window->DrawList->AddText( bb.Min + ImVec2( 25, 5 ), ImColor( 220, 220, 220 ), label.c_str( ) );
    }
}