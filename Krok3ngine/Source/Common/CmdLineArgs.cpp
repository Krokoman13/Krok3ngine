#include "Krok3ngine.h"
#include "CmdLineArgs.h"
#include <algorithm>

void CmdLineArgs::ReadArguments() {
	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	for (int i = 1; i < argc; ++i) {
		std::wstring key = argv[i];
		if (key[0] != '-') continue;
		key.erase(0, 1);
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
		ReadArgument(key.c_str());
	}
}

void KROK3NGINE_API CmdLineArgs::ReadArgument(const WCHAR* argument) {
	if (wcscmp(argument, L"debug") == 0) {
		Engine::SetMode(Engine::EngineMode::DEBUG);
	} else if (wcscmp(argument, L"editor") == 0) {
		Engine::SetMode(Engine::EngineMode::EDITOR);
	} else if (wcscmp(argument, L"server") == 0) {
		Engine::SetMode(Engine::EngineMode::SERVER);
	}
}
