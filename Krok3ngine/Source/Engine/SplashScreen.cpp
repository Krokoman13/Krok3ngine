#include "Krok3ngine.h"
#include "SplashScreen.h"

namespace SplashScreen {
	SplashWindow* m_splashWindow;

	VOID Open() {
		if (m_splashWindow != nullptr) {
			return;
		}

		m_splashWindow = new SplashWindow();
	}

	VOID Close() {

	}

	VOID AddMessage(const WCHAR* p_message) {

	}
}


SplashWindow::SplashWindow() 
	: Win32::Window(L"SplashScreen", L"SplashScreen", NULL, 500, 600) {
	wcscpy_s(m_OutputMessage, L"SplashScreen Starting...");
	Win32::Window::RegisterNewClass();
	Win32::Window::Initialize();
}

SplashWindow::~SplashWindow() {
}

LRESULT SplashWindow::MessageHandler(HWND p_hWnd, UINT p_message, WPARAM p_wparam, LPARAM p_lparam) {
	return CommonMessageHandler(p_hWnd, p_message, p_wparam, p_lparam);
}
