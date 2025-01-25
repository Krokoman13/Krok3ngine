#pragma once

namespace Win32 {
	class KROK3NGINE_API SubObject {
	public:
		SubObject(const std::wstring& a_className, const std::wstring& a_classTitle, HICON a_hIcon = NULL);
		~SubObject();

	public:
		virtual VOID RegisterNewClass();
		virtual VOID Initialize() = 0;

	protected:
		static LRESULT CALLBACK SetupMessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam);
		static LRESULT AssignMessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam);
		virtual LRESULT MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam);
		
	public:
		VOID SetClass(const std::wstring& a_className);
		std::wstring GetClass();
		VOID SetTitle(const std::wstring& a_classTitle);
		std::wstring GetTitle();

		HWND GetHandle() { return handle; }

	private:
		VOID setHandle(HWND a_handle) { handle = a_handle; }

	private:
		std::wstring m_class;
		std::wstring m_title;

		HICON m_hIcon;
		HWND handle;		
		

	};
}