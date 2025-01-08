#include "Krok3ngine.h"
#include "SplashScreen.h"

#include "Platform/Win32/Win32Utils.h"

namespace SplashScreen {
	#define WM_OUTPUTMESSAGE (WM_USER + 0x0001)

	SplashWindow* m_splashWindow;

	VOID Open() {
		if (m_splashWindow != nullptr) {
			return;
		}

		m_splashWindow = new SplashWindow();
	}

	VOID Close() {

	}

	VOID AddMessage(const WCHAR* a_message) {
		PostMessage(m_splashWindow->GetHandle(), WM_OUTPUTMESSAGE, (WPARAM)a_message, 0);
	}
}


SplashWindow::SplashWindow() 
	: Win32::Window(L"SplashScreen", L"SplashScreen", 500, 600) {
	wcscpy_s(m_outputMessage, L"SplashScreen Starting...");
	Win32::Window::RegisterNewClass();
	Win32::Window::Initialize();
}

SplashWindow::~SplashWindow() {
}

LRESULT SplashWindow::MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) {
	switch (a_message) {
	case WM_PAINT:
	{
		HDC hdc;
		PAINTSTRUCT ps;

		hdc = BeginPaint(a_hWnd, &ps);

		Win32::Utils::AddBitmap(PerGameSettings::SplashURL(), hdc);

		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));

		if (Engine::GetMode() != Engine::EngineMode::RELEASE) {
			std::wstring egineModeText = Engine::EngineModeString() + L" Mode";
			SetTextAlign(hdc, TA_RIGHT);
			TextOut(hdc, m_width - 15, 15, egineModeText.c_str(), (INT)wcslen(egineModeText.c_str()));
		}

		SetTextAlign(hdc, TA_CENTER);
		TextOut(hdc, m_width / 2, m_height - 30, m_outputMessage, (INT)wcslen(m_outputMessage));
		EndPaint(a_hWnd, &ps);
	}
	break;
	case WM_OUTPUTMESSAGE:
	{

		WCHAR* msg = (WCHAR*)a_wparam;
		wcscpy_s(m_outputMessage, msg);
		RedrawWindow(GetHandle(), NULL, NULL, RDW_INVALIDATE);
		return 0;
	}
	break;
	}

	return CommonMessageHandler(a_hWnd, a_message, a_wparam, a_lparam);
}
