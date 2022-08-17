#include "../hooks.h"

void __stdcall hooks::client_dll::frame_stage_notify::hook(e_client_frame_stage stage) {

	if (stage == FRAME_RENDER_START
		&& interfaces::m_engine->is_in_game()) {
		interfaces::m_engine->get_view_angles(globals::angles::m_view);
	}
	else if ( stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START && interfaces::m_engine->is_in_game( ) )
	{
			shared->send_data( );
	}

	frame_stage_notify_original(stage);
}

__declspec (naked) void __stdcall hooks::client_dll::create_move::gate(int sequence_number, float input_sample_frame_time, bool active) {
	__asm {
		push ebp
		mov ebp, esp
		push ebx
		push esp
		push dword ptr[active]
		push dword ptr[input_sample_frame_time]
		push dword ptr[sequence_number]
		call hooks::client_dll::create_move::hook
		pop ebx
		pop ebp
		retn 0Ch
	}
}

void __stdcall hooks::client_dll::create_move::hook(int sequence_number, float input_sample_frame_time, bool active, bool& packet) {
	create_move_original(sequence_number, input_sample_frame_time, active);

	globals::m_packet = packet = true;

	if (!globals::m_local)
		return;

	const auto cmd = interfaces::m_input->get_user_cmd(sequence_number);
	if (!cmd
		|| !cmd->m_command_number)
		return;

	globals::m_cur_cmd = cmd;
	globals::m_pressing_move = (cmd->m_buttons.has(IN_LEFT) || cmd->m_buttons.has(IN_FORWARD) || cmd->m_buttons.has(IN_BACK) ||
		cmd->m_buttons.has(IN_RIGHT) || cmd->m_buttons.has(IN_MOVELEFT) || cmd->m_buttons.has(IN_MOVERIGHT) ||
		cmd->m_buttons.has(IN_JUMP));

	movement->set_view_angles(cmd->m_view_angles);

	engine_prediction->update();

	movement->on_create_move(false);

	fake_lags->on_create_move( );

	engine_prediction->process();


	aimbot->on_create_move( );

	/* */

	engine_prediction->restore();

	cmd->m_view_angles.sanitize();

	globals::angles::m_anim = cmd->m_view_angles;

	movement->on_create_move(true);

	packet = globals::m_packet;

	const auto verified = interfaces::m_input->get_verified_user_cmd(sequence_number);

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->get_check_sum();
}

void __fastcall hooks::client_dll::level_init_pre_entity::hook( void* ecx, void* edx, const char* map_name )
{
	fake_lags->reset_data( );

	return hooks::level_init_pre_entity_original( ecx, map_name );
}
