#pragma once
#include "../globals.h"

class c_menu : public c_singleton<c_menu> {
public:
	void on_paint();
private:
	int m_selected_tab = 0;
	int m_selected_subtab[6] = {};
};
#define menu c_menu::instance()