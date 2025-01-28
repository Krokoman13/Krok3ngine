#pragma once

#include <Uxtheme.h>
#pragma comment(lib,"uxtheme.lib")

namespace Win32 {
	class KROK3NGINE_API Window : public Win32::SubObject, public Win32::Caption{
	public:
		Window(std::wstring a_title, WindowType a_type = WindowType::POPUP, HICON a_hIcon = NULL);
		~Window();

	public:
		virtual VOID Initialize() override;
		virtual LRESULT MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) override;

	public:
		SIZE GetSize() { return m_size; }
		VOID SetSize(SIZE a_size) { m_size = a_size; }
		VOID SetSize(INT a_width, INT a_height) { m_size.cx = a_width; m_size.cy = a_height; }

		BOOL GetFocused() { return m_isFocused; }
		VOID SetFocused(BOOL a_focused) { m_isFocused = a_focused; }

	private:
		VOID onNonClientCreate();
		VOID onNonClientActive(BOOL a_active);
		VOID onNonClientPaint(HRGN a_hRegion);
		VOID redrawWindow();
		VOID paintCaption(HDC a_hdc);
		VOID onNonClientLeftMouseButtonDown();
		VOID onGetMinMaxInfo(MINMAXINFO* minmax);
		VOID onExitSizeMove();
		VOID onPaint();

	private:
		SIZE m_size;
		WindowType m_type;

		BOOL m_isFocused = false;
	};
}