#include "Krok3ngine.h"
#include "Subobject.h"

namespace Win32 {

	SubObject::SubObject(const std::wstring& p_className, const std::wstring& p_classTitle, HICON p_hIcon)
		: m_class(p_className), m_title(p_classTitle), m_hIcon(p_hIcon){
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
		wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wcex.hIcon = m_hIcon;
		wcex.hIconSm = m_hIcon;
		wcex.lpszClassName = m_class.c_str();
		wcex.lpszMenuName = nullptr;
		wcex.hInstance = HInstance();
		wcex.lpfnWndProc = SetupMessageHandler;
		RegisterClassEx(&wcex);
	}
	LRESULT SubObject::SetupMessageHandler(HWND p_hWnd, UINT p_message, WPARAM p_wparam, LPARAM p_lparam) {
		if (p_message != WM_NCCREATE) {
			return DefWindowProc(p_hWnd, p_message, p_wparam, p_lparam);
		}

		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(p_lparam);
		SubObject* const pWnd = static_cast<SubObject*>(pCreate->lpCreateParams);
		SetWindowLongPtr(p_hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(p_hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&AssignMessageHandler));
	}

	LRESULT SubObject::AssignMessageHandler(HWND p_hWnd, UINT p_message, WPARAM p_wparam, LPARAM p_lparam) {
		SubObject* const pWnd = reinterpret_cast<SubObject*>(GetWindowLongPtr(p_hWnd, GWLP_USERDATA));
		return pWnd->MessageHandler(p_hWnd, p_message, p_wparam, p_lparam);
	}

	LRESULT SubObject::CommonMessageHandler(HWND p_hWnd, UINT p_message, WPARAM p_wparam, LPARAM p_lparam) {
		return DefWindowProc(p_hWnd, p_message, p_wparam, p_lparam);
	}
}
