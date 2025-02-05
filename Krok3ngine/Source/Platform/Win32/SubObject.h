#pragma once

namespace Win32 {
	class KROK3NGINE_API SubObject : public Engine::InitializableObject<> {
	public:
		SubObject(const wchar_t* a_className, const wchar_t* a_classTitle, HICON a_hIcon = NULL);
		~SubObject();

	public:
		void RegisterNewClass();

	protected:
		void baseInitialize() { RegisterNewClass(); };
		static LRESULT CALLBACK SetupMessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam);
		static LRESULT AssignMessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam);
		virtual LRESULT MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam);
		
	public:
		const wchar_t* GetClass() { return m_class; };
		const wchar_t* GetTitle() { return m_title; };
		HWND GetHandle() { return m_handle; }

	private:
		const wchar_t* m_class;
		const wchar_t* m_title;

		HICON m_hIcon;
		HWND m_handle;		
	};
}