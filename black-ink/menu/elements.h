#pragma once

#include "../utils/render/render.h"

#include "../utils/imgui/imgui.h"
#include "../utils/imgui/imgui_internal.h"

#include "../utils/imgui/imgui_tweaker.hpp"

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
        ImVec2 size = CalcItemSize( {100, 50}, 100, 50 );

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
        window->DrawList->AddImage( texture, bb.Min + ImVec2( 42,  8), bb.Min + ImVec2(58, 24) );
        window->DrawList->AddText( bb.Min + ImVec2(50 - label_size.x / 2, 30), ImColor(220, 220, 220 ), label.c_str());
	}
}