#pragma once
#include "../../../globals.h"

class c_model {
private:
	c_texture* m_texture;
	c_texture* m_cubemap = nullptr;
	float preview_matrix = 180.f;
	c_merged_mdl* m_model;
public:

	c_texture* get_preview_texture( )
	{
		return m_texture;
	}

	void on_render( );
};
inline c_model g_Model;
#define model g_Model;