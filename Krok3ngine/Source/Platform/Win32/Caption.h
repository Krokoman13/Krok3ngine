#pragma once

#define			CB_CLOSE		0
#define			CB_MINIMIZE		1
#define			CB_MAXIMIZE		2

namespace Win32 {
	class Caption
	{
	public:
		Caption();
		~Caption();

	public:
		struct CaptionButton {
			std::wstring text = L"x";
			INT width;
			INT command = -1;
			RECT rect = RECT{};

			CaptionButton(std::wstring a_text, INT a_command, INT a_width = 50) {
				text = a_text;
				width = a_width;
				command = a_command;
			};
		};

	public:
		VOID CreateAndAddCaptionButton(std::wstring a_text, INT a_command, INT a_width = 50);
		const std::list<CaptionButton*>& GetCaptionButtons() { return m_captionButtons; };

		VOID SetShowTitle(BOOL a_ShowTitle) { m_showTitle = a_ShowTitle; }
		BOOL GetShowTitle() { return m_showTitle; }

	private:
		BOOL m_showTitle = TRUE;
		std::list<CaptionButton*> m_captionButtons;
	};
}
