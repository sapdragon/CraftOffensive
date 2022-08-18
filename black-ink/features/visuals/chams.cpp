#include "../features.h"
#include "../../hooks/hooks.h"

void c_chams::create_material( chams_layer material ) {
	const auto key_values = reinterpret_cast< c_key_values* >( interfaces::m_mem_alloc->alloc( 36u ) );

	key_values->init( material.shader_type.data( ) );
	key_values->load_from_buffer( material.file_name.data( ), material.material_data.data( ) );

	material.mat = interfaces::m_material_system->create_material( material.file_name.data( ), key_values );
	if ( material.mat != nullptr )
		material.inited = true;
 
	materials.push_back( material );
}

void c_chams::create_material(std::string_view material_name, std::string_view label, bool buildin, std::string_view shader_type, std::string_view material_data) {
	auto data = chams_layer{};

	data.file_name = material_name;
	data.shader_type = shader_type;
	data.buildin = buildin;
	data.material_data = material_data;
	data.label = label;

	const auto key_values = reinterpret_cast<c_key_values*>(interfaces::m_mem_alloc->alloc(36u));

	key_values->init(shader_type.data());
	key_values->load_from_buffer(material_name.data(), material_data.data());

	data.mat = interfaces::m_material_system->create_material( material_name.data( ), key_values );

	materials.push_back( data );
}

void c_chams::override_material(int type, const col_t& clr, bool ignorez ) {
	i_material* material = materials[ type ].mat;

	material->set_flag( MATERIAL_FLAG_IGNOREZ, ignorez );

	material->alpha_modulate(clr.a() / 255.f);
	material->color_modulate(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f);

	if (const auto $envmaptint = material->find_var(_("$envmaptint"), nullptr, false)) {
		$envmaptint->set_value(vec3_t(clr.r() / 255.f, clr.g() / 255.f, clr.b() / 255.f));
	}

	interfaces::m_model_render->forced_material_override(material);
}

void c_chams::draw_material_on_entity( chams_array visible, chams_array invisible, i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones )
{
	if ( !invisible.empty( ) ) {
		for ( auto& material_options : invisible )
		{
			if ( !material_options.m_enable )
				continue;

			override_material( material_options.m_material, material_options.m_color, true );
			hooks::draw_model_execute_original( ecx, context, state, info, bones );
		}
	}


	if ( !visible.empty( ) ) {
		for ( auto& material_options : visible )
		{
			if ( !material_options.m_enable )
				continue;

			override_material( material_options.m_material, material_options.m_color, false );
			hooks::draw_model_execute_original( ecx, context, state, info, bones );
		}
	}
}

bool c_chams::on_draw_model( i_model_render* ecx, void* context, const draw_model_state_t& state, const model_render_info_t& info, matrix3x4_t* bones ) {
	const auto entity = reinterpret_cast< c_base_entity* >( interfaces::m_entity_list->get_client_entity( info.m_index ) );
	if ( !entity )
		return true;

	if ( strstr( info.m_model->m_name, _( "player" ) ) )
	{
		const auto player = reinterpret_cast< c_cs_player* >( entity );
		if ( !player ||!player->is_alive( ) || !entity->is_player( ) )
				return true;
		
		//else if ( m_shared_players.contains( player->get_index( ) ) )
		//	draw_material_on_entity( get_shared_player( player->get_index( ) ), ecx, context, state, info, bones );

		if ( player == (c_cs_player*) globals::m_local )
			draw_material_on_entity( cfg::local_player_visible, cfg::local_player_invisible, ecx, context, state, info, bones );
		else if ( player->is_enemy(globals::m_local) )
			draw_material_on_entity( cfg::enemy_visible, cfg::enemy_invisible, ecx, context, state, info, bones );
		else
			draw_material_on_entity( cfg::teammates_visible, cfg::teammates_invisible, ecx, context, state, info, bones );
	}
	return true;
}
