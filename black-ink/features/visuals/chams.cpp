#include "../features.h"
#include "../../hooks/hooks.h"

i_material* c_chams::create_material(std::string_view material_name, std::string_view shader_type, std::string_view material_data) {
	const auto key_values = reinterpret_cast<c_key_values*>(interfaces::m_mem_alloc->alloc(36u));

	key_values->init(shader_type.data());
	key_values->load_from_buffer(material_name.data(), material_data.data());

	return interfaces::m_material_system->create_material(material_name.data(), key_values);
}

void c_chams::override_material(int type, const col_t& clr, bool ignorez ) {
	i_material* material = nullptr;

	switch (type) {
	case MATERIAL_TYPE_REGULAR: material =  m_regular; break;
	case MATERIAL_TYPE_FLAT: material = ignorez ? m_flat_z : m_flat; break;
	}

	material->set_flag( MATERIAL_FLAG_IGNOREZ, ignorez );

	material->alpha_modulate(clr.a() / 255.f);
	material->color_modulate(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f);

	if (const auto $envmaptint = material->find_var(_("$envmaptint"), nullptr, false)) {
		$envmaptint->set_value(vec3_t(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f));
	}

	interfaces::m_model_render->forced_material_override(material);
}

bool c_chams::on_draw_model( i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones ) {
	const auto entity = reinterpret_cast< c_base_entity* >( interfaces::m_entity_list->get_client_entity( info.m_index ) );
	if ( !entity )
		return true;

	if ( strstr( info.m_model->m_name, _( "player" ) ) )
	{
		const auto player = reinterpret_cast< c_cs_player* >( entity );
		if ( !player->is_alive( ) || !entity->is_player( ) )
				return true;

			//std::array < chams_settings_t, 10 > chams_settings = cfg::get< std::array < chams_settings_t, 10 > >( FNV1A( "chams.player.layers" ) );

		if ( player == (c_cs_player*) globals::m_local )
		{

			override_material( MATERIAL_TYPE_REGULAR, col_t::palette_t::green( ), false );

			hooks::draw_model_execute_original( ecx, context, state, info, bones );

		}
		else if ( player->is_enemy(globals::m_local) )
		{
			override_material( MATERIAL_TYPE_REGULAR, col_t::palette_t::red( ), true );

			hooks::draw_model_execute_original( ecx, context, state, info, bones );

			override_material( MATERIAL_TYPE_REGULAR, col_t::palette_t::red( ), false );
			
		
			return true;
		}
		else
		{
			override_material( MATERIAL_TYPE_REGULAR, col_t::palette_t::blue( ), true );

			hooks::draw_model_execute_original( ecx, context, state, info, bones );

			override_material( MATERIAL_TYPE_REGULAR, col_t::palette_t::blue( ), false );
		}
	}
	return true;
}
