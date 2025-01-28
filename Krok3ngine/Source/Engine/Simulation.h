#pragma once

#include "DirectX/DirectXManager.h"

namespace Krok3ngine {
	class KROK3NGINE_API Simulation : public Win32::IApplication, public DX::DirectXManager {
	public:
		Simulation();
		~Simulation();

	public:
		VOID PreInitialize() override;
		VOID Initialize() override;
		VOID Update() override;

		virtual LRESULT MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) override;
	};
}