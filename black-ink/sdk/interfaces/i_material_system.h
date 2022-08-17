#pragma once


class i_material_system {
public:
	VFUNC(create_material(const char* name, c_key_values* key_values), 83, i_material*(__thiscall*)(void*, const char*, c_key_values*), name, key_values)
	VFUNC(find_material(const char* name, const char* group, bool complain = true, const char* complain_prefix = nullptr), 84,
		i_material*(__thiscall*)(void*, const char*, const char*, bool, const char*), name, group, complain, complain_prefix)

	VFUNC( get_back_buffer_format( ), 36, int( __thiscall* )( void* ) )



	VFUNC(first_material(), 86, uint16_t(__thiscall*)(void*))
	VFUNC(next_material(uint16_t handle), 87, uint16_t(__thiscall*)(void*, uint16_t), handle)
	VFUNC(invalid_material(), 88, uint16_t(__thiscall*)(void*))
	VFUNC(get_material(uint16_t handle), 89, i_material*(__thiscall*)(void*, uint16_t), handle)
	VFUNC(get_num_materials(), 90, int(__thiscall*)(void*))
	VFUNC( begin_render_target_allocation( ), 94, void( __thiscall* )( void* ) )
	VFUNC( end_render_target_allocation( ), 95, void( __thiscall* )( void* ) )
	VFUNC(CreateNamedRenderTargetTextureEx( const char* pRTName, int w, int h, RenderTargetSizeMode_t sizeMode, int format, MaterialRenderTargetDepth_t depth, unsigned int textureFlags, unsigned int renderTargetFlags ), 97, c_texture*( __thiscall* )( void*, const char*, int, int, RenderTargetSizeMode_t, int, MaterialRenderTargetDepth_t, unsigned int, unsigned int), pRTName, w, h, sizeMode, format, depth, textureFlags, renderTargetFlags )
	VFUNC(get_render_context( ), 115, i_material_render_context* ( __thiscall* )( void* ) );

	VFUNC(finish_render_target_allocation( ), 136, void( __thiscall* )( void* ) );

};