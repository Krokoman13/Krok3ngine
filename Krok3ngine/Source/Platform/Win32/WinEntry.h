#include "Krok3ngine.h"
#include "IApplication.h"

extern IApplication* EntryApplication();

INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	auto entryApp = EntryApplication();

	PerGameSettings pergameSettings;
	entryApp->SetupPerGameSettings();
	Logger logger;
	entryApp->Initialize();

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages then process them.
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			entryApp->Update();
		}
	}

	return 0;
}
