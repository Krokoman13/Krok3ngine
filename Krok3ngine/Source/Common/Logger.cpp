#include "Krok3ngine.h"
#include <fstream>
#include <Shlobj.h>
#include <cstdio>

Logger* Logger::m_inst;

Logger::Logger() {
	if (m_inst) return;
	m_inst = this;
}

Logger::~Logger() {
}

void Logger::PrintLog(const WCHAR* fmt, ...) {
	WCHAR buf[4096];
	va_list args;

	va_start(args, fmt);
	vswprintf_s(buf, fmt, args);
	va_end(args);

	std::wstring s = dateLog() + L" " + buf + L'\n';
	log(s.c_str());
}

void Logger::PrintWarning(const WCHAR* fmt, ...)
{
	WCHAR buf[4096];
	va_list args;

	va_start(args, fmt);
	vswprintf_s(buf, fmt, args);
	va_end(args);

	std::wstring s =
		L'\n' +
		dateLog() + L" Warning: " + buf + L"\n\n";

	log(s.c_str());
}

void Logger::PrintError(const WCHAR* fmt, ...)
{
	WCHAR buf[4096];
	va_list args;

	va_start(args, fmt);
	vswprintf_s(buf, fmt, args);
	va_end(args);

	const std::wstring line = L"\n------------------------------------------------------------------------------------\n";

	std::wstring s = 
		L'\n' + 
		line + 
		dateLog() + L" ERROR: " + buf + L'\n'
		+ line;

	log(s.c_str());
}

void Logger::log(const WCHAR* a_str)
{
	if (a_str == nullptr) return;

	std::wfstream outfile;
	outfile.open(std::wstring(LogDirectory() + L"/" + LogFile()), std::ios_base::app);

	OutputDebugString(a_str);

	if (!outfile.is_open()) {
		MessageBox(NULL, L"Unable to open log file...", L"Log Error", MB_OK);
		return;
	}

	outfile << a_str;
	outfile.close();
}

std::wstring Logger::dateLog()
{
	return L"[" + Time::GetDateTimeString() + L"]";
}

std::wstring Logger::LogDirectory()
{
	WCHAR path[1024];
	WCHAR* appDataLocal;
	SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &appDataLocal);
	wcscpy_s(path, appDataLocal);
	wcscat_s(path, L"\\");
	wcscat_s(path, PerGameSettings::GameName());
	CreateDirectory(path, NULL);
	wcscat_s(path, L"\\Log");
	CreateDirectory(path, NULL);
	return path;
}

std::wstring Logger::LogFile()
{
	WCHAR file[1024];
	wcscpy_s(file, PerGameSettings::GameName());
	wcscat_s(file, PerGameSettings::BootTime());
	wcscat_s(file, L".log");
	return file;
}

void Logger::PrintDebugSeperator()
{
	std::wstring s = L"\n------------------------------------------------------------------------------------\n\n";
	log(s.c_str());
}