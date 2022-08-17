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

#include "api/md5/MD5.hpp"

auto GetCPUId = [ ] ( ) -> std::string
{
	int CPUInfo[ 4 ] = { -1 };
	char CPUBrandString[ 0x40 ];
	__cpuid( CPUInfo, 0x80000000 );
	unsigned int nExIds = CPUInfo[ 0 ];

	memset( CPUBrandString, 0, sizeof( CPUBrandString ) );

	for ( size_t i = 0x80000000; i <= nExIds; ++i )
	{
		__cpuid( CPUInfo, i );
		if ( i == 0x80000002 )
			memcpy( CPUBrandString, CPUInfo, sizeof( CPUInfo ) );
		else if ( i == 0x80000003 )
			memcpy( CPUBrandString + 16, CPUInfo, sizeof( CPUInfo ) );
		else if ( i == 0x80000004 )
			memcpy( CPUBrandString + 32, CPUInfo, sizeof( CPUInfo ) );
	}

	return std::string( CPUBrandString );
};

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

		cloud->user_profile.m_hwid = md5(GetCPUId( ));
	}

	catch (const std::runtime_error& error) {
		MessageBoxA(nullptr, error.what(), "black-ink | fatal error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(module), 0);
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(module), 0);
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