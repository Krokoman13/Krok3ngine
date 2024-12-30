#pragma once
#include <string>

namespace Time {
	std::wstring KROK3NGINE_API GetTime(BOOL stripped = FALSE);
	std::wstring KROK3NGINE_API GetDate(BOOL stripped = FALSE);
	std::wstring KROK3NGINE_API GetDateTimeString(BOOL stripped = FALSE);
}