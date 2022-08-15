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

		const auto send_datagram = reinterpret_cast< void* >(utils::find_pattern_from_module( GetModuleHandleA( _( "engine.dll" ) ), _( "55 8B EC 83 E4 F0 B8 38 01 10 00 E8" ) ) ) ;

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

		if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
			throw std::runtime_error("failed to enable all hooks.");
	}

	void undo() {
		MH_Uninitialize();
		MH_DisableHook(MH_ALL_HOOKS);
	}
}
