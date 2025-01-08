#include "Krok3ngine.h"
#include "Window.h"

namespace Win32 {
	Window::Window(std::wstring a_className, std::wstring a_classTitle, INT a_width, INT a_height, HICON a_hIcon)
		: SubObject(a_className, a_classTitle, a_hIcon), m_width(a_width), m_height(a_height) {
	}

	Window::~Window() {
	}

	VOID Window::Initialize() {
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		RECT r = { 0, 0, m_width, m_height };
		AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, false);
		int width = r.right - r.left;
		int heigth = r.bottom - r.top;

		HWND handle = CreateWindow(GetClass().c_str(), GetTitle().c_str(),
			WS_POPUP, (desktop.right - m_width) / 2, (desktop.bottom - m_height) / 2, m_width, m_height, 0, 0, HInstance(), (void*)this);
		SetHandle(handle);

		ShowWindow(handle, SW_SHOW);
		UpdateWindow(handle);
	}
}
