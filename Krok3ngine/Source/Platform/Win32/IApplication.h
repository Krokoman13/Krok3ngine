#pragma once

#define ENTRYAPP(x) Win32::IApplication* EntryApplication() { return new x; }

namespace Win32 {
	class KROK3NGINE_API IApplication : public Engine::KEngineObject<> {
	public:
		IApplication();
		virtual ~IApplication() {};

	public:
		virtual void SetupPerGameSettings() = 0;
	};

	IApplication* EntryApplication();
}