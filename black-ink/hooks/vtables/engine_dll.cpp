#pragma once
#include "../hooks.h"

void __cdecl hooks::engine_dll::host_run_frame_input::hook( void*, void*, float accumulated_extra_samples, bool final_tick )
{
	shared->on_run_frame_input( );
	return host_run_frame_input_original( accumulated_extra_samples, final_tick );
}