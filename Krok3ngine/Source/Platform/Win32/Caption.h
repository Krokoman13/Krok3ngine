#pragma once

#define			CB_CLOSE		0
#define			CB_MINIMIZE		1
#define			CB_MAXIMIZE		2

namespace Win32 {
	class KROK3NGINE_API Caption {
	public:
		Caption();
		~Caption();

	public:
		struct CaptionButton {
			std::wstring text = L"x";
			int width;
			int command = -1;
			RECT rect = RECT{};

			CaptionButton(std::wstring a_text, int a_command, int a_width = 50) {
				text = a_text;
				width = a_width;
				command = a_command;
			};
		};

	public:
		void CreateAndAddCaptionButton(std::wstring a_text, int a_command, int a_width = 50);
		const std::list<CaptionButton*>& GetCaptionButtons() { return m_captionButtons; };

		void SetShowTitle(bool a_ShowTitle) { m_showTitle = a_ShowTitle; }
		bool GetShowTitle() { return m_showTitle; }

	private:
		bool m_showTitle = TRUE;
		std::list<CaptionButton*> m_captionButtons;
	};
}
