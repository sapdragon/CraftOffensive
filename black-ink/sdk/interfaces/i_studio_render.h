#pragma once
struct draw_model_info_t
{
	studiohdr_t* m_studiohdr;
	void* m_hardware_data;
	void* m_decals;
	int32_t m_skin;
	int32_t m_body;
	int32_t m_hitbox_set;
	i_client_renderable* m_client_entity;
	int32_t m_lod;
	void* m_color_meshes;
	bool m_static_lighting;
};

enum class OverrideType_t {
	OVERRIDE_NORMAL,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
	OVERRIDE_SSAO_DEPTH_WRITE = 4
};

class i_studio_render {
	std::byte pad_0[ 0x250 ];
	i_material* m_material_override;
	std::byte pad_1[ 0xC ];
	OverrideType_t override_type;
public:
	VFUNC( set_ambient_light_colors( const vec4_t* ambient_only_colors ), 20, void( __thiscall* )( void*, const vec4_t* ), ambient_only_colors );
	VFUNC( set_local_lights( int32_t num_lights, const void* lights ), 22, void( __thiscall* )( void*, int32_t, const void* ), num_lights, lights );
	VFUNC( ForcedMaterialOverride( i_material* material, OverrideType_t type = OverrideType_t::OVERRIDE_NORMAL, int32_t index = -1 ), 33, void( __thiscall* )( void*, i_material*, OverrideType_t, int32_t ), material, type, index );

	bool IsForcedMaterialOverride( ) {
		if ( !m_material_override )
			return override_type == OverrideType_t::OVERRIDE_DEPTH_WRITE || override_type == OverrideType_t::OVERRIDE_SSAO_DEPTH_WRITE;
		return strstr( m_material_override->get_name( ), "dev/glow" );
	}
};