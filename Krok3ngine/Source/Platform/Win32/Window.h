#pragma once

#include <Uxtheme.h>
#pragma comment(lib,"uxtheme.lib")

//Todo: Allow for more window customization

namespace Win32 {
	class KROK3NGINE_API Window : public Win32::SubObject, public Win32::Caption{
	public:
		Window(const wchar_t* a_title, WindowType a_type = WindowType::POPUP, HICON a_hIcon = NULL);
		~Window();

	public:
		virtual LRESULT MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) override;

	protected:
		virtual void baseInitialize() override;

	public:
		SIZE GetSize() { return m_size; }
		void SetSize(SIZE a_size) { m_size = a_size; }
		void SetSize(int a_width, int a_height) { m_size.cx = a_width; m_size.cy = a_height; }

		bool GetFocused() { return m_isFocused; }
		void SetFocused(bool a_focused) { m_isFocused = a_focused; }

	private:
		void onNonClientCreate();
		void onNonClientActive(bool a_active);
		void onNonClientPaint(HRGN a_hRegion);
		void redrawWindow();
		void paintCaption(HDC a_hdc);
		void onNonClientLeftMouseButtonDown();
		void onGetMinMaxInfo(MINMAXINFO* minmax);
		void onExitSizeMove();
		void onPaint();

	private:
		SIZE m_size;
		WindowType m_type;

		bool m_isFocused = false;
	};
}