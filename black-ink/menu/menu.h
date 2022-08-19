#pragma once
#include "../globals.h"

#include "../utils/imgui/TextEditor.h"

class c_menu : public c_singleton<c_menu> {
public:
	void on_paint();

	int selected_material = 0;
	int selected_chams_tab = 0;
	int selected_esp_tab = 0;

	ImDrawList* draw;
	ImVec2 pos;

private:
	int m_selected_tab = 0;
	int m_selected_subtab[6] = {};

	TextEditor editor;
};
#define menu c_menu::instance()