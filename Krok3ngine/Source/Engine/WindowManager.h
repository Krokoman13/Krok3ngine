#pragma once

namespace Engine {
	class KROK3NGINE_API WindowManager : private Win32::Window, public Event::EventDispatcher {
	public:
		WindowManager(std::wstring a_title, Win32::WindowType a_type = Win32::WindowType::POPUP, HICON a_hIcon = NULL);

	public:
		Event::Event<SIZE>* onWindowResize;

	public:
		void Initialize() override;
		virtual LRESULT MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) override;

	public:
		Win32::Window* GetWindow() { return this; };
		HWND GetWindowHandle() { return this->GetHandle(); };

	private:
	};
}