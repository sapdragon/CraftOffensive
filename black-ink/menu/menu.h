#pragma once
#include "../globals.h"

#include "../utils/imgui/TextEditor.h"
#include "../utils/imgui/imgui_tweaker.hpp"

class c_menu : public c_singleton<c_menu> {
public:
	void on_paint();

	int selected_material = 0;
	int selected_chams_tab = 0;
	int selected_esp_tab = 0;

	bool resetAnimation = false;

	ImDrawList* draw;
	ImVec2 pos;

	int m_selected_tab = 0;
	int m_selected_subtab[ 6 ] = {};

	ImAnimations::Animator<const char*, float> m_animator;
private:

	TextEditor editor;
};
#define menu c_menu::instance()