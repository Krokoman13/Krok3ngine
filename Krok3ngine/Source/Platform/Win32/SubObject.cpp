#include "Krok3ngine.h"
#include "Subobject.h"

namespace Win32 {

	SubObject::SubObject(const std::wstring& a_className, const std::wstring& a_classTitle, HICON a_hIcon)
		: m_class(a_className), m_title(a_classTitle), m_hIcon(a_hIcon), handle() {
	}

	SubObject::~SubObject() {
	}

	VOID SubObject::RegisterNewClass() {
		WNDCLASSEX wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(CreateSolidBrush(RGB(36, 36, 36)));
		wcex.hIcon = m_hIcon;
		wcex.hIconSm = m_hIcon;
		wcex.lpszClassName = m_class.c_str();
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
		pWnd->setHandle(a_hWnd);
		return pWnd->MessageHandler(a_hWnd, a_message, a_wparam, a_lparam);
	}

	LRESULT SubObject::AssignMessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) {
		SubObject* const pWnd = reinterpret_cast<SubObject*>(GetWindowLongPtr(a_hWnd, GWLP_USERDATA));
		return pWnd->MessageHandler(a_hWnd, a_message, a_wparam, a_lparam);
	}

	LRESULT SubObject::MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) {
		return DefWindowProc(a_hWnd, a_message, a_wparam, a_lparam);
	}

	VOID SubObject::SetClass(const std::wstring& a_className) {
		m_class = a_className;
	}

	std::wstring SubObject::GetClass() {
		return m_class;
	}

	VOID SubObject::SetTitle(const std::wstring& a_classTitle) {
		m_title = a_classTitle;
	}

	std::wstring SubObject::GetTitle()
	{
		return m_title;
	}
}
