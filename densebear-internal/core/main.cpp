#include "../dependencies/utilities/csgo.hpp"
#include "features/misc/misc.hpp"
#include "menu/gui.hpp"
#include "../dependencies/utilities/events/events.hpp"

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	console::initialize(variables::densebear::console);
#endif

	try {
		interfaces::initialize();
		render::initialize();
		gui::initialize();
		hooks::initialize();
		
		/* customs */
		events.initialize();

		std::cout << "[+] " + variables::densebear::client + " " + variables::densebear::version + ": Initialized Successfully!\n";
	}

	catch (const std::runtime_error& error) {
		MessageBoxA(nullptr, error.what(), "Initialized Failure!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	while (!(GetAsyncKeyState(VK_END) & 1) && !variables::densebear::terminate)
		std::this_thread::sleep_for(std::chrono::milliseconds(200));

	variables::menu::opened = false;
	std::this_thread::sleep_for(std::chrono::milliseconds(200)); //originally 50ms but 200ms almost always prevents crashing
	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	hooks::release();
	gui::release();

	/* customs */
	events.release();

#ifdef _DEBUG
	console::release();
#endif
	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);

	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(handle);
		break;
	}

	case DLL_PROCESS_DETACH: {
		release();
		break;
	}
	}

	return true;
}