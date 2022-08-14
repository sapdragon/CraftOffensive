#include "../hooks.h"

void __stdcall hooks::client_mode::override_view::hook(view_setup_t* view) {
	override_view_original(view);
}