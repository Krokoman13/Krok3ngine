#include "Krok3ngine.h"
#include "Window.h"

namespace Win32 {
	Window::Window(std::wstring p_className, std::wstring p_classTitle, HICON p_hIcon, INT p_width, INT p_height)
		: SubObject(p_className, p_classTitle, p_hIcon), m_width(p_width), m_height(p_height) {
	}

	Window::~Window() {
	}

	VOID Window::Initialize() {
		HWND hWnd = CreateWindow(m_class.c_str(), m_title.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, m_width, m_height, nullptr, nullptr, HInstance(), (void*)this);

		if (!hWnd) {
			MessageBox(0, L"Failed to Create Window!.", 0, 0);
			PostQuitMessage(0);
			return;
		}

		ShowWindow(hWnd, SW_SHOW);
	}
}
