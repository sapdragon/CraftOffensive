#include "common_includes.h"
#include "hooks/hooks.h"
#include "events/events.h"
#include "globals.h"

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

		long long amongus = 0x69690004C201B0;
		static std::string sig = ("55 8B EC 56 8B F1 33 C0 57 8B 7D 08");

		LPCWSTR modules[] {
			L"client.dll",
			L"engine.dll",
			L"server.dll",
			L"studiorender.dll",
			L"materialsystem.dll"
		};

		for (auto base : modules) {
			WriteProcessMemory(GetCurrentProcess(), utils::find_pattern_from_module(GetModuleHandleW(base), sig), &amongus, 5, 0);
		}

		interfaces::init();
		input::init();
		render::init();
		netvars::init();
		cfg::init();
		hooks::init();
		events::init();
		shared->init( );
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