#pragma once

namespace Krok3ngine {
	class KROK3NGINE_API Simulation : public Win32::IApplication, public Win32::Window {
	public:
		Simulation();
		~Simulation();

	public:
		virtual VOID PreInitialize() override;
		virtual LRESULT MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) override;
	};
}