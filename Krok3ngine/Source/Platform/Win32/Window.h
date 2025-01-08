#pragma once

#include "SubObject.h"

namespace Win32 {
	class KROK3NGINE_API Window : public Win32::SubObject {
	public:
		Window(std::wstring a_className, std::wstring a_classTitle, INT a_width, INT a_height, HICON a_hIcon = NULL);
		~Window();

	public:
		virtual VOID Initialize() override;

	protected:
		INT m_width;
		INT	m_height;
	};
}