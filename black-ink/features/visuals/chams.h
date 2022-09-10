#pragma once
#include "../../globals.h"
#include <array>


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
	void override_material( int type, const col_t& clr, bool ignorez );

private:

	void draw_material_on_entity( chams_material_settings_t visible, chams_material_settings_t invisible, i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones );

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

		create_material(_("black_ink_metallic.vmt"), _( "Metallic" ), true, _("VertexLitGeneric"), _(R"#("VertexLitGeneric" {
      "$basetexture" "vgui/white_additive"
      "$ignorez" "0"
      "$envmap" "env_cubemap"
      "$normalmapalphaenvmapmask" "1"
      "$envmapcontrast"  "1"
      "$nofog" "1"
      "$model" "1"
      "$nocull" "0"
      "$selfillum" "1"
      "$halflambert" "1"
      "$znearer" "0"
      "$flat" "1" 
})#"));

		create_material(_("black_ink_oil.vmt"), _( "Oil" ), true, _("VertexLitGeneric"), _(R"#("VertexLitGeneric" {
		"$basetexture" "VGUI/white_additive"
		"$bumpmap" "de_nuke/hr_nuke/pool_water_normals_002"
		"$bumptransform" "center 0.5 0.5 scale 0.25 0.25 rotate 0.0 translate 0.0 0.0"
		"$ignorez" "0"
		"$nofog" "1"
		"$model" "1"
		"$color2" "[1.0, 1.0, 1.0]"
		"$halflambert" "1"
		"$envmap" "env_cubemap"
		"$envmaptint" "[0.1 0.1 0.1]"
		"$envmapfresnel" "1.0"
		"$envmapfresnelminmaxexp" "[1.0, 1.0, 1.0]"
		"$phong" "1"
		"$phongexponent" "1024"
		"$phongboost" "4.0"
		"$phongfresnelranges" "[1.0, 1.0, 1.0]"
		"$rimlight" "1"
		"$rimlightexponent" "4.0"
		"$rimlightboost" "2.0"
    
		"Proxies"
		{
			"TextureScroll"
			{
				"textureScrollVar" "$bumptransform"
				"textureScrollRate" "0.25"
				"textureScrollAngle" "0.0"
			}
		}
	})#"));
	}

	//chams_entity_settings_t get_shared_player( int player_index ) { return m_shared_players[ player_index ]; }
	//chams_entity_settings_t set_shared_chams( int player_index, chams_entity_settings_t settings ) { return m_shared_players[ player_index ] = settings; }

	bool on_draw_model(i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones);
};
#define chams c_chams::instance()