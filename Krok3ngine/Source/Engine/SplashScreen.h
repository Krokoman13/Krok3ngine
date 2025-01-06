#pragma once

#include "Platform/Win32/Window.h"

namespace SplashScreen {
	VOID KROK3NGINE_API Open();
	VOID KROK3NGINE_API Close();
	VOID KROK3NGINE_API AddMessage(const WCHAR* p_message);
}

class KROK3NGINE_API SplashWindow : public Win32::Window {
public:
	SplashWindow();
	~SplashWindow();

protected:
	virtual LRESULT MessageHandler(HWND p_hWnd, UINT p_message, WPARAM p_wparam, LPARAM p_lparam) override;

private:
	WCHAR m_OutputMessage[MAX_NAME_STRING];
};