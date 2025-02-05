#include "Krok3ngine.h"
#include "Subobject.h"

namespace Win32 {
	SubObject::SubObject(const wchar_t* a_className, const wchar_t* a_classTitle, HICON a_hIcon)
		: m_class(a_className), m_title(a_classTitle), m_hIcon(a_hIcon), m_handle() {
	}

	SubObject::~SubObject() {
		//Todo: Unregister class
		delete m_title;
		delete m_class;
	}

	void SubObject::RegisterNewClass() {
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(Color::grayBackground));
		wcex.hIcon = m_hIcon;
		wcex.hIconSm = m_hIcon;
		wcex.lpszClassName = m_class;
		wcex.lpszMenuName = nullptr;
		wcex.hInstance = HInstance();
		wcex.lpfnWndProc = SetupMessageHandler;
		RegisterClassEx(&wcex);
	}

	LRESULT SubObject::SetupMessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) {
		if (a_message != WM_NCCREATE) {
			return DefWindowProc(a_hWnd, a_message, a_wparam, a_lparam);
		}

		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(a_lparam);
		SubObject* const pWnd = static_cast<SubObject*>(pCreate->lpCreateParams);
		SetWindowLongPtr(a_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(a_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&AssignMessageHandler));
		pWnd->m_handle = a_hWnd;
		return pWnd->MessageHandler(a_hWnd, a_message, a_wparam, a_lparam);
	}

	LRESULT SubObject::AssignMessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) {
		SubObject* const pWnd = reinterpret_cast<SubObject*>(GetWindowLongPtr(a_hWnd, GWLP_USERDATA));
		return pWnd->MessageHandler(a_hWnd, a_message, a_wparam, a_lparam);
	}

	LRESULT SubObject::MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) {
		return DefWindowProc(a_hWnd, a_message, a_wparam, a_lparam);
	}
}
