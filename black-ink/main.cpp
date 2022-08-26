#include "common_includes.h"
#include "hooks/hooks.h"
#include "events/events.h"
#include "globals.h"
#include "api/api.h"
#include <intrin.h>

int __stdcall undo() {
	events::undo();
	hooks::undo();
	input::undo();

	return 1;
}

unsigned long __stdcall init(LPVOID module) {
	do {
		memory::get_all_modules();

		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	} while (memory::m_modules.find(FNV1A("serverbrowser.dll")) == memory::m_modules.end());

	try {
		globals::m_local = *SIG("client.dll", "8B 0D ? ? ? ? 83 FF FF 74 07").self_offset(0x2).cast<c_local_player*>();

		interfaces::init();
		input::init();
		render::init();
		netvars::init();
		cfg::init();
		shared->init( );
		hooks::init();
		events::init();
	}
	catch (const std::runtime_error& error) {
		MessageBoxA(nullptr, error.what(), "craftoffensive | fatal error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(module), 0);
	}
	
	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );

	FreeLibraryAndExitThread( static_cast< HMODULE >( module ), 0 );
}

int __stdcall DllMain(HMODULE module, unsigned long reason_for_call, void* reserved) {
	DisableThreadLibraryCalls(module);

	switch (reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, init, module, NULL, nullptr))
			CloseHandle(handle);

		break;
	}

	case DLL_PROCESS_DETACH: {
		undo();
		break;
	}
	}

	return 1;
}