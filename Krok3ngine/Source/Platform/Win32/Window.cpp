#include "Krok3ngine.h"
#include "Window.h"

#include <UxTheme.h>
#pragma comment(lib, "uxtheme.lib")

#define DCX_USESTYLE 0x00010000

namespace Win32 {
	Window::Window(std::wstring a_title, WindowType a_type, HICON a_hIcon)
		: SubObject(a_title, a_title, a_hIcon), m_type(a_type) {
		SetSize(DEFAULTWIDTH, DEFAULTHEIGHT);
	}

	Window::~Window() {
	}

	VOID Window::Initialize() {
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);

		RECT r = { 0, 0, GetSize().cx, GetSize().cy };
		AdjustWindowRect(&r, m_type, false);
		int width = r.right - r.left;
		int heigth = r.bottom - r.top;

		HWND handle = CreateWindow(GetClass().c_str(), GetTitle().c_str(),
			m_type, (desktop.right - m_size.cx) / 2, (desktop.bottom - m_size.cy) / 2, m_size.cx, m_size.cy, 0, 0, HInstance(), (void*)this);

		ShowWindow(handle, SW_SHOW);
		UpdateWindow(handle);
	}

	LRESULT Window::MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) {
		switch (a_message) {
		case WM_NCCREATE: { onNonClientCreate(); }										return TRUE;
		case WM_NCACTIVATE: { onNonClientActive(LOWORD(a_wparam) != WA_INACTIVE); }		return TRUE;
		case WM_NCPAINT: { onNonClientPaint((HRGN)a_wparam); }							return FALSE;
		case WM_NCLBUTTONDOWN: { onNonClientLeftMouseButtonDown(); }					break;
		case WM_NCLBUTTONDBLCLK: { Win32::Utils::MaximizeWindow(GetHandle()); }         return 0;

		case WM_GETMINMAXINFO: { onGetMinMaxInfo(((MINMAXINFO*)a_lparam)); }			return 0;
		case WM_EXITSIZEMOVE: { onExitSizeMove(); }										break;
		
		case WM_PAINT: { onPaint(); }													break;

		case WM_TIMER: { redrawWindow(); }												break;
		}

		return SubObject::MessageHandler(a_hWnd, a_message, a_wparam, a_lparam);;
	}

	VOID Window::onNonClientCreate() {
		SetTimer(GetHandle(), 1, 100, NULL);
		SetWindowTheme(GetHandle(), L"", L"");
		Win32::Utils::ModifyClassStyle(GetHandle(), 0, CS_DROPSHADOW);

		Win32::Caption::CreateAndAddCaptionButton(L"x", CB_CLOSE, 30);
		Win32::Caption::CreateAndAddCaptionButton(L"🗖", CB_MAXIMIZE, 30);
		Win32::Caption::CreateAndAddCaptionButton(L"🗕", CB_MINIMIZE, 30);
	}

	VOID Window::onNonClientActive(BOOL a_active) {
		SetFocused(a_active);
	}

	VOID Window::onNonClientPaint(HRGN a_hRegion) {
		// start draw
		HDC hdc = GetDCEx(GetHandle(), a_hRegion, DCX_WINDOW | DCX_INTERSECTRGN | DCX_USESTYLE);

		RECT rect;
		GetWindowRect(GetHandle(), &rect);

		SIZE size = SIZE{ rect.right - rect.left, rect.bottom - rect.top };

		HBITMAP hbmMen = CreateCompatibleBitmap(hdc, size.cx, size.cy);
		HANDLE hOld = SelectObject(hdc, hbmMen);

		// draw
		HBRUSH brush = CreateSolidBrush(RGB(46, 48, 46));

		RECT newRect = RECT{ 0, 0, size.cx, size.cy };

		FillRect(hdc, &newRect, brush);

		if (GetFocused() && !Win32::Utils::IsWindowFullscreen(GetHandle())) {
			brush = CreateSolidBrush(RGB(0, 100, 150));
			FrameRect(hdc, &newRect, brush);
		}

		paintCaption(hdc);

		DeleteObject(brush);

		// end draw

		BitBlt(hdc, 0, 0, size.cx, size.cy, hdc, 0, 0, SRCCOPY);
		SelectObject(hdc, hOld);
		DeleteObject(hbmMen);

		ReleaseDC(GetHandle(), hdc);
	}

	VOID Window::redrawWindow() {
		SetWindowPos(GetHandle(), 0, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_DRAWFRAME | SWP_FRAMECHANGED); // reset window
		SendMessage(GetHandle(), WM_PAINT, 0, 0);
	};

	VOID Window::paintCaption(HDC a_hdc) {
		RECT rect;
		GetWindowRect(GetHandle(), &rect);

		SIZE size = SIZE{ rect.right - rect.left, rect.bottom - rect.top };

		if (GetShowTitle()) {
			rect = RECT{ 0,0, size.cx, 30 };

			SetBkMode(a_hdc, TRANSPARENT);
			SetTextColor(a_hdc, GetFocused() ? RGB(255, 255, 255) : RGB(128, 128, 128));

			DrawText(a_hdc, GetTitle().c_str(), wcslen(GetTitle().c_str()), &rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}

		INT margin = 5;
		INT padding = 0;
		INT spacing = margin;

		POINT cursorPos;
		GetCursorPos(&cursorPos);

		GetWindowRect(GetHandle(), &rect);

		cursorPos.x -= rect.left;
		cursorPos.y -= rect.top;

		for (auto& button : Caption::GetCaptionButtons()) {
			button->rect = RECT{ size.cx - spacing - button->width, margin, size.cx - spacing , 30 - margin };
			spacing += button->width + padding;

			if (button->rect.left < cursorPos.x && button->rect.right > cursorPos.x &&
				button->rect.top < cursorPos.y && button->rect.bottom > cursorPos.y) {

				HBRUSH brush = CreateSolidBrush(RGB(92, 92, 92));
				FillRect(a_hdc, &button->rect, brush);
				DeleteObject(brush);
			}

			if (button->text.compare(L"🗖") == 0 && Win32::Utils::IsWindowFullscreen(GetHandle())) {
				button->text = L"🗗";
			}
			else if (button->text.compare(L"🗗") == 0 && !Win32::Utils::IsWindowFullscreen(GetHandle())) {
				button->text = L"🗖";
			}

			DrawText(a_hdc, button->text.c_str(), wcslen(button->text.c_str()), &button->rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		}
	}

	VOID Window::onNonClientLeftMouseButtonDown() {
		POINT cursorPos;
		GetCursorPos(&cursorPos);

		RECT rect;
		GetWindowRect(GetHandle(), &rect);

		cursorPos.x -= rect.left;
		cursorPos.y -= rect.top;

		for (auto& button : Caption::GetCaptionButtons()) {
			if (button->rect.left < cursorPos.x && button->rect.right > cursorPos.x &&
				button->rect.top < cursorPos.y && button->rect.bottom > cursorPos.y) {

				switch (button->command) {
				case CB_CLOSE: { SendMessage(GetHandle(), WM_CLOSE, 0, 0); } break;
				case CB_MINIMIZE: { ShowWindow(GetHandle(), SW_MINIMIZE); } break;
				case CB_MAXIMIZE: { Win32::Utils::MaximizeWindow(GetHandle()); } break;
				}
			}		
		}
	};

	VOID Window::onGetMinMaxInfo(MINMAXINFO* minmax) {
		RECT WorkArea; SystemParametersInfo(SPI_GETWORKAREA, 0, &WorkArea, 0);
		minmax->ptMaxSize.x = (WorkArea.right - WorkArea.left);
		minmax->ptMaxSize.y = (WorkArea.bottom - WorkArea.top);
		minmax->ptMaxPosition.x = WorkArea.left;
		minmax->ptMaxPosition.y = WorkArea.top;
		minmax->ptMinTrackSize.x = 400;
		minmax->ptMinTrackSize.y = 300;
	}

	VOID Window::onExitSizeMove() {
		RECT rect;
		GetWindowRect(GetHandle(), &rect);
		RECT workArea;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
		if (rect.top < workArea.top + 10 && !Win32::Utils::IsWindowFullscreen(GetHandle())) {
			Win32::Utils::MaximizeWindow(GetHandle());
		}
	}

	VOID  Window::onPaint() {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(GetHandle(), &ps);

		RECT rc;
		GetClientRect(GetHandle(), &rc);

		HBRUSH brush = CreateSolidBrush(RGB(36, 36, 36));

		FillRect(hdc, &rc, brush);

		DeleteObject(brush);

		EndPaint(GetHandle(), &ps);
	}
}
