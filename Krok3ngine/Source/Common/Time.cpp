#include "Krok3ngine.h"
#include <ctime>
#include <sstream>
#include <iomanip>

/* Get Time in format '00:00:00' */
/* Stripped = 000000 */
std::wstring Time::GetTime(bool stripped)
{
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	std::wstringstream wss;
	wss << std::put_time(&ltm, L"%T");

	std::wstring timeString = wss.str();

	if (stripped) {
		std::wstring chars = L":";
		for (WCHAR c : chars) {
			timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
		}
	}
	return timeString;
}

/* Get date in format '00/00/00' */
/* Stripped = 000000 */
std::wstring Time::GetDate(bool stripped) {
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	std::wstringstream wss;
	wss << std::put_time(&ltm, L"%d/%m/%y");
	std::wstring timeString = wss.str();

	if (stripped) {
		std::wstring chars = L"/";
		for (WCHAR c : chars) {
			timeString.erase(std::remove(timeString.begin(), timeString.end(), c), timeString.end());
		}
	}

	return timeString;
}

/* Get date time in format '00/00/00 00:00:00' */
/* Stripped = 000000000000 */
std::wstring Time::GetDateTimeString(bool stripped) {
	std::wstring timeString = GetDate(stripped) + (stripped ? L"" : L" ") + GetTime(stripped);
	return timeString;
}
