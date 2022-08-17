#pragma once
class i_render_view {
public:
	VFUNC(set_blend(float value), 4, void(__thiscall*)(void*, float), value)
	VFUNC(set_clr(float* value), 6, void(__thiscall*)(void*, float*), value)
	VFUNC(push_3d_view( i_material_render_context* render_context, const view_setup_t& view, int32_t flags, c_texture* render_target, frustum_t frustum_planes ), 44, void( __thiscall* )( void*, i_material_render_context*, const view_setup_t&, int32_t, c_texture*, frustum_t ), render_context, view, flags, render_target, frustum_planes );
	VFUNC(pop_view( i_material_render_context* render_context, frustum_t frustum_planes ), 46, void( __thiscall* )( void*, i_material_render_context*, frustum_t ), render_context, frustum_planes );

};