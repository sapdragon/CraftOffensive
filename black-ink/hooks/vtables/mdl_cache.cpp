#pragma once
#include "../hooks.h"

unsigned short __fastcall hooks::mdl_cache::find_mdl::hook( void* ecx, void* edx, char* path )
{
	return hooks::find_mdl_original(ecx, path);
}
