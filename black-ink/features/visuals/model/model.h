#pragma once
#include "../../../globals.h"

class c_model {
private:
	c_texture* m_texture;
	c_texture* m_cubemap = nullptr;
	float preview_matrix = 180.f;
	c_merged_mdl* m_model;

	c_texture* m_text_texture;
	c_merged_mdl* m_text_model;
public:

	c_texture* get_preview_texture( )
	{
		return m_texture;
	}

	c_texture* get_text_preview_texture( )
	{
		return m_text_texture;
	}

	void on_render( chams_material_settings_t chams_mat );
	void on_text_render( chams_material_settings_t chams_mat );
};
inline c_model g_Model;
#define model g_Model;