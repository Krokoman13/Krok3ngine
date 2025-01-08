#pragma once

#include "Platform/Win32/Window.h"

namespace SplashScreen {
	VOID KROK3NGINE_API Open();
	VOID KROK3NGINE_API Close();
	VOID KROK3NGINE_API AddMessage(const WCHAR* a_message);
}

class KROK3NGINE_API SplashWindow : public Win32::Window {
public:
	SplashWindow();
	~SplashWindow();

protected:
	virtual LRESULT MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) override;

private:
	WCHAR m_outputMessage[MAX_NAME_STRING];
};