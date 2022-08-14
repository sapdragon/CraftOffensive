#include "../hooks.h"

void __fastcall hooks::surface::lock_cursor::hook(i_surface* ecx, void* edx) {
	return input::m_blocked ? ecx->unlock_cursor() : lock_cursor_original(ecx, edx);
}