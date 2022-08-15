#include "../hooks.h"

int __fastcall hooks::networking::send_datagram::hook( void* ecx, uint32_t edx, void* data )
{
	return hooks::send_datagram_original( ecx, edx, data );
}