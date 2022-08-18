#pragma once
#include "../../globals.h"
#include <array>

using chams_array = std::vector < chams_material_settings_t>;

struct chams_layer {
	i_material* mat;
	std::string shader_type;
	std::string material_data;
	std::string label;
	std::string file_name;

	bool buildin = true;
	bool inited = false;
};

class c_chams : public c_singleton<c_chams> {
public:
	std::vector < chams_layer > materials;
	void create_material( std::string_view material_name, std::string_view label, bool buildin, std::string_view shader_type, std::string_view material_data );
	void create_material( chams_layer material );


private:

	void override_material(int type, const col_t& clr, bool ignorez);
	void draw_material_on_entity( chams_array visible, chams_array invisible, i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones );

	//std::map < int, chams_entity_settings_t > m_shared_players;
public:
	c_chams() {
		create_material(_("black_ink_regular.vmt"), _("Regular" ), true,  _( "VertexLitGeneric" ), _( R"#("VertexLitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$ignorez"      "0"
	"$model"		"1"
	"$flat"			"0"
	"$nocull"		"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$wireframe"	"0"
})#"));

		create_material(_("black_ink_flat.vmt"), _( "Flat" ), true, _("UnlitGeneric"), _(R"#("UnlitGeneric" {
	"$basetexture" "vgui/white_additive"
	"$ignorez"      "0"
	"$model"		"1"
	"$flat"			"1"
	"$nocull"		"1"
	"$selfillum"	"1"
	"$halflambert"	"1"
	"$nofog"		"1"
	"$wireframe"	"0"
})#"));
	}

	//chams_entity_settings_t get_shared_player( int player_index ) { return m_shared_players[ player_index ]; }
	//chams_entity_settings_t set_shared_chams( int player_index, chams_entity_settings_t settings ) { return m_shared_players[ player_index ] = settings; }

	bool on_draw_model(i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones);
};
#define chams c_chams::instance()