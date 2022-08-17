#include "hooks.h"

namespace hooks {
	void init() {
		const auto reset_index = reinterpret_cast<void*>(get_virtual(interfaces::m_d3d_device, 16u));
		const auto present_index = reinterpret_cast<void*>(get_virtual(interfaces::m_d3d_device, 17u));
		const auto frame_stage_notify_index = reinterpret_cast<void*>(get_virtual(interfaces::m_client_dll, 37u));
		const auto create_move_index = reinterpret_cast<void*>(get_virtual(interfaces::m_client_dll, 22u));
		const auto override_view_index = reinterpret_cast<void*>(get_virtual(interfaces::m_client_mode, 18u));
		const auto draw_model_execute_index = reinterpret_cast<void*>(get_virtual(interfaces::m_model_render, 21u));
		const auto paint_traverse_index = reinterpret_cast<void*>(get_virtual(interfaces::m_panel, 41u));
		const auto eye_angles_index = reinterpret_cast<void*>(get_virtual(c_cs_player::get_vtable(), 170u));
		const auto lock_cursor_index = reinterpret_cast<void*>(get_virtual(interfaces::m_surface, 67u));
		const auto set_mdl_index = reinterpret_cast< void* >( get_virtual( interfaces::m_model_cache, 10u ) );
		const auto level_init_pre_entity = reinterpret_cast< void* >( get_virtual( interfaces::m_client_dll, 5u ) );



		const auto send_datagram = reinterpret_cast< void* >(utils::find_pattern_from_module( GetModuleHandleA( _( "engine.dll" ) ), _( "55 8B EC 83 E4 F0 B8 38 01 10 00 E8" ) ) ) ;
		const auto process_spotted_entity_update = reinterpret_cast< void* >( utils::find_pattern_from_module( GetModuleHandleA( _( "client.dll" ) ), _( "55 8B EC 83 EC 18 8B 45 08 53 56 57 80 78 18 00" ) ) );
		const auto allocate_memory = reinterpret_cast< void* >( utils::find_pattern_from_module( GetModuleHandleA( _( "vstdlib.dll" ) ), _( "55 8B EC A1 ? ? ? ? 8B 08 8B 01 5D FF 60 04" ) ) );
		auto host_run_frame_input = utils::find_pattern_from_module( GetModuleHandleA( _(  "engine.dll" ) ) , _( "E8 ? ? ? ? F3 0F 10 45 ? F2 0F 10 4D ?" ) );
		host_run_frame_input = memory::address_t( host_run_frame_input ).rel32( );
		const auto packet_end = reinterpret_cast< void* >( utils::find_pattern_from_module( GetModuleHandleA( _( "engine.dll" ) ), _( "56 8B F1 E8 ? ? ? ? 8B 8E ? ? ? ? 3B 8E ? ? ? ?" ) ) );
		const auto cl_move = reinterpret_cast< void* >( utils::find_pattern_from_module( GetModuleHandleA( _( "engine.dll" ) ), _( "55 8B EC 81 EC 64 01 00 00 53 56 8A F9" ) ) );

		//file_system = ( void*) ( (uintptr_t) ( file_system) + 0x2 );

		const auto loose_file_allowed_index = reinterpret_cast< void* >( get_virtual( interfaces::m_file_system, 128u ) );
		const auto get_unverified_file_hashes_index = reinterpret_cast< void* >( get_virtual( interfaces::m_file_system, 101u ) );


		if (MH_Initialize() != MH_OK)
			throw std::runtime_error("failed to initialize MH_Initialize.");

		if (MH_CreateHook(reset_index, &d3d_device::reset::hook, reinterpret_cast<void**>(&reset_original)) != MH_OK)
			throw std::runtime_error("failed to initialize d3d_device_reset.");

		if (MH_CreateHook(present_index, &d3d_device::present::hook, reinterpret_cast<void**>(&present_original)) != MH_OK)
			throw std::runtime_error("failed to initialize d3d_device_present.");

		if (MH_CreateHook(frame_stage_notify_index, &client_dll::frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify_original)) != MH_OK)
			throw std::runtime_error("failed to initialize frame_stage_notify.");

		if (MH_CreateHook(create_move_index, &client_dll::create_move::gate, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
			throw std::runtime_error("failed to initialize create_move.");

		if (MH_CreateHook(override_view_index, &client_mode::override_view::hook, reinterpret_cast<void**>(&override_view_original)) != MH_OK)
			throw std::runtime_error("failed to initialize override_view.");

		if (MH_CreateHook(draw_model_execute_index, &model_render::draw_model_execute::hook, reinterpret_cast<void**>(&draw_model_execute_original)) != MH_OK)
			throw std::runtime_error("failed to initialize draw_model_execute.");

		if (MH_CreateHook(paint_traverse_index, &panel::paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
			throw std::runtime_error("failed to initialize paint_traverse.");

		if (MH_CreateHook(eye_angles_index, &player::eye_angles::hook, reinterpret_cast<void**>(&eye_angles_original)) != MH_OK)
			throw std::runtime_error("failed to initialize eye_angles.");

		if (MH_CreateHook(lock_cursor_index, &surface::lock_cursor::hook, reinterpret_cast<void**>(&lock_cursor_original)) != MH_OK)
			throw std::runtime_error("Failed to initialize lock_cursor.");

		if ( MH_CreateHook( send_datagram, &networking::send_datagram::hook, reinterpret_cast< void** >( &send_datagram_original ) ) != MH_OK )
			throw std::runtime_error( "Failed to initialize send_datagram" );

		if ( MH_CreateHook( process_spotted_entity_update, &other::process_spotted_entity_update::hook, reinterpret_cast< void** >( &process_spotted_entity_update_original ) ) != MH_OK )
			throw std::runtime_error( "Failed to initialize process_spotted_entity_update." );

		if ( MH_CreateHook( allocate_memory, &other::allocate_memory::hook, reinterpret_cast< void** >( &allocate_memory_original ) ) != MH_OK )
			throw std::runtime_error( "Failed to initialize allocate_memory." );

		if ( MH_CreateHook( host_run_frame_input, &engine_dll::host_run_frame_input::hook, reinterpret_cast< void** >( &host_run_frame_input_original ) ) != MH_OK )
			throw std::runtime_error( "Failed to initialize host_run_frame_input." );

		if ( MH_CreateHook( packet_end, &networking::packet_end::hook, reinterpret_cast< void** >( &packet_end_original ) ) != MH_OK )
			throw std::runtime_error( "Failed to initialize packet_end." );

		if ( MH_CreateHook( set_mdl_index, &mdl_cache::find_mdl::hook, reinterpret_cast< void** >( &find_mdl_original ) ) != MH_OK )
			throw std::runtime_error( "Failed to initialize set_mdl." );

		if ( MH_CreateHook( cl_move, &networking::cl_move::hook, reinterpret_cast< void** >( &cl_move_original ) ) != MH_OK )
			throw std::runtime_error( "Failed to initialize cl_move." );

		if ( MH_CreateHook( level_init_pre_entity, &client_dll::level_init_pre_entity::hook, reinterpret_cast< void** >( &level_init_pre_entity_original ) ) != MH_OK )
			throw std::runtime_error( "Failed to initialize level_init_pre_entity." );

		if ( MH_CreateHook( loose_file_allowed_index, &other::loose_file_allowed::hook, reinterpret_cast< void** >( &loose_file_allowed_original ) ) != MH_OK )
			throw std::runtime_error( "Failed to initialize loose_file_allowed." );

		if ( MH_CreateHook( get_unverified_file_hashes_index, &other::get_unverified_file_hashes::hook, nullptr) != MH_OK )
			throw std::runtime_error( "Failed to initialize get_unvirifed_file_hashes." );
	

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
			throw std::runtime_error("failed to enable all hooks.");
	}

	void undo() {
		MH_Uninitialize();
		MH_DisableHook(MH_ALL_HOOKS);
	}
}
