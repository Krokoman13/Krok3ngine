#pragma once
#include <string>

namespace Time {
	std::wstring KROK3NGINE_API GetTime(bool stripped = FALSE);
	std::wstring KROK3NGINE_API GetDate(bool stripped = FALSE);
	std::wstring KROK3NGINE_API GetDateTimeString(bool stripped = FALSE);
}