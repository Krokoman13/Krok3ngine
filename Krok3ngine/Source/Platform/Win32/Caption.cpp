#include "Krok3ngine.h"
#include "Caption.h"

namespace Win32 {
	Caption::Caption(){ 
	}

	Caption::~Caption() {
		while (!m_captionButtons.empty()) {
			delete m_captionButtons.back();
			m_captionButtons.pop_back();
		}
	}

	VOID Caption::CreateAndAddCaptionButton(std::wstring a_text, INT a_command, INT a_width) {
		m_captionButtons.push_back(new CaptionButton(a_text, a_command, a_width));
	}
}
