#pragma once
#include "../hooks.h"

bool __fastcall hooks::other::process_spotted_entity_update::hook( void* ecx, void* edx, c_process_spotted_entity_update* message )
{
	dormant->on_radar_data_received( message );
	return hooks::process_spotted_entity_update_original( ecx, edx, message );
}

void* __fastcall hooks::other::allocate_memory::hook( void* ecx, void* edx, int size )
{
	DWORD64 dwPattern = *( DWORD64* ) ( ( DWORD ) ( _ReturnAddress( ) ) - 0x15 );
	if ( dwPattern == 0x15FF0000008B840F || dwPattern == 0x15FF00000098850F )
		return nullptr;

	return allocate_memory_original(ecx, edx, size);
}
