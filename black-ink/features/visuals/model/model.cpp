#include "model.h"

void c_model::on_render( )
{
    if ( !m_texture ) {
        interfaces::m_material_system->begin_render_target_allocation( );
        m_texture = interfaces::m_material_system->CreateNamedRenderTargetTextureEx( "esp_preview", 300, 400, RenderTargetSizeMode_t::RT_SIZE_NO_CHANGE, interfaces::m_material_system->get_back_buffer_format( ), MATERIAL_RT_DEPTH_SHARED, 0x4 | 0x8, 0x1 );
        interfaces::m_material_system->finish_render_target_allocation( );
    }

    if ( !m_model ) {
        c_merged_mdl* new_model = static_cast< c_merged_mdl* >( interfaces::m_mem_alloc->alloc( 0x75C ) );
        static auto create_model_addr = utils::find_pattern_from_module( GetModuleHandleA( _( "client.dll" ) ), _( "53 8B D9 56 57 8D 4B 04 C7 03 ? ? ? ? E8 ? ? ? ? 6A" ) );
        reinterpret_cast< void( __thiscall* )( void* ) >( create_model_addr )( new_model );

        if ( !new_model )
            return;

        m_model = new_model;

        m_model->set_mdl( "models/player/custom_player/uiplayer/animset_uiplayer.mdl" );
        m_model->set_merge_mdl( "models/player/custom_player/legacy/tm_balkan_variantg.mdl" );
        m_model->set_sequence( 214, false );
        m_model->setup_bones_for_attachment_queries( );
        m_model->set_merge_mdl( "models/weapons/w_rif_ak47.mdl" );
    }

    m_model->m_root_mdl.m_time += interfaces::m_global_vars->m_frame_time / 2.f;

    auto render_context = interfaces::m_material_system->get_render_context( );

    view_setup_t view_setup;
    memset( &view_setup, 0, sizeof( view_setup ) );
    view_setup.m_x = 50;
    view_setup.m_y = 50;
    view_setup.m_width = 300;
    view_setup.m_height = 400;
    view_setup.m_fov = 100.f;
    view_setup.m_origin = vec3_t( -69.f, -9.f, 29.f );
    view_setup.m_angles = qangle_t( 0.0f, 0.0f, 0.0f );
    view_setup.m_near = 7.0f;
    view_setup.m_far = 1000.0f;

    render_context->push_render_target_and_viewport( );
    render_context->set_render_target( m_texture );
    render_context->clear_color_4ub( 0, 0, 0, 0 );
    render_context->clear_buffers( true, true, true );

    render_context->set_lighting_origin( -115.0f, 0.0f, 40.0f );
    render_context->set_int_rendering_parameter( 10, 0 );

    static vec4_t white[ 6 ] = { { 0.4f, 0.4f, 0.4f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f }, { 0.4f, 0.4f, 0.4f, 1.0f } };
    interfaces::m_studio_render->set_ambient_light_colors( white );
    interfaces::m_studio_render->set_local_lights( 0, nullptr );

    frustum_t dummy_frustum;
    interfaces::m_render_view->push_3d_view( render_context, view_setup, 0, m_texture, dummy_frustum );
    interfaces::m_model_render->suppress_engine_lighting( true );

    float_t color[ 3 ] = { 1.0f, 1.0f, 1.0f };
    interfaces::m_render_view->set_clr( color );
    interfaces::m_render_view->set_blend( 1.0f );

    matrix3x4_t mat_player_view;
    math::angle_matrix( { 0.0f, preview_matrix, 90.0f }, mat_player_view, { 0.0f, 0.0f, 0.0f } );

    m_model->draw( mat_player_view );

    interfaces::m_model_render->suppress_engine_lighting( false );
    interfaces::m_render_view->pop_view( render_context, dummy_frustum );

    render_context->bind_local_cubemap( nullptr );
    render_context->pop_render_target_and_viewport( );

    render_context->release( );
}
