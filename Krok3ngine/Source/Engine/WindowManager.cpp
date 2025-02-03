#include "Krok3ngine.h"

namespace Engine {
	WindowManager::WindowManager(std::wstring a_title, Win32::WindowType a_type, HICON a_hIcon)
		: Win32::Window(a_title, a_type, a_hIcon) {
		onWindowResize = createEvent<SIZE>();
	}

	void WindowManager::Initialize() {
		RegisterNewClass();
		Initialize();
	}

	LRESULT WindowManager::MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) {
		switch (a_message) {
			case WM_SIZE: {
				onWindowResize->Post(SIZE(LOWORD(a_lparam), HIWORD(a_lparam)));
			}
			break;
		}
		return Win32::Window::MessageHandler(a_hWnd, a_message, a_wparam, a_lparam);
	}
}
