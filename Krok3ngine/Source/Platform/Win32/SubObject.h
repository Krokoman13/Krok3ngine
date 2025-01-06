#pragma once

namespace Win32 {
	class KROK3NGINE_API SubObject {
	public:
		SubObject(const std::wstring& p_className, const std::wstring& p_classTitle, HICON p_hIcon);
		~SubObject();

	public:
		virtual VOID RegisterNewClass();
		virtual VOID Initialize() = 0;

	protected:
		static LRESULT CALLBACK SetupMessageHandler(HWND p_hWnd, UINT p_message, WPARAM p_wparam, LPARAM p_lparam);
		static LRESULT AssignMessageHandler(HWND p_hWnd, UINT p_message, WPARAM p_wparam, LPARAM p_lparam);
		virtual LRESULT CommonMessageHandler(HWND p_hWnd, UINT p_message, WPARAM p_wparam, LPARAM p_lparam);

		virtual LRESULT MessageHandler(HWND p_hWnd, UINT p_message, WPARAM p_wparam, LPARAM p_lparam) = 0;

	protected:
		std::wstring m_class;
		std::wstring m_title;

		HICON m_hIcon;
	};
}