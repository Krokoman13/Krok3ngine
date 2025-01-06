#pragma once

#include "SubObject.h"

namespace Win32 {
	class KROK3NGINE_API Window : public Win32::SubObject {
	public:
		Window(std::wstring p_className, std::wstring p_classTitle, HICON p_hIcon, INT p_width, INT p_height);
		~Window();

	public:
		virtual VOID Initialize() override;

	private:
		INT m_width;
		INT	m_height;
	};
}