#include "../hooks.h"

void __fastcall hooks::model_render::draw_model_execute::hook(i_model_render* ecx, void* edx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones) {
	if (info.m_flags.has(STUDIO_SHADOW_DEPTH_TEXTURE))
		return draw_model_execute_original(ecx, context, state, info, bones);

	if (chams->on_draw_model(ecx, context, state, info, bones)) {
		draw_model_execute_original(ecx, context, state, info, bones);
	}

	interfaces::m_model_render->forced_material_override();
}
