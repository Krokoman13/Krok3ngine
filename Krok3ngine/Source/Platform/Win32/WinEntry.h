#include "Krok3ngine.h"

#include "IApplication.h"
#include "Common/CmdLineArgs.h"

extern Win32::IApplication* EntryApplication();

INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	Logger logger;

	auto entryApp = EntryApplication();

	PerGameSettings pergameSettings;
	entryApp->SetupPerGameSettings();

	CmdLineArgs::ReadArguments();

	entryApp->PreInitialize();
	entryApp->Initialize();

	MSG msg = { 0 };
	while (msg.message != WM_QUIT) {
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			entryApp->Update();
		}
	}

	return 0;
}
