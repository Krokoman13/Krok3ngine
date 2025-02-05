#pragma once
#include <string>

class KROK3NGINE_API Logger {
	//Singleton class that handles logging
private:
	static Logger* m_inst;
public:
	static Logger* Instance() { return m_inst; }

public:
	Logger();
	~Logger();

public:
	static void PrintLog(const WCHAR* fmt, ...);
	static void PrintWarning(const WCHAR* fmt, ...);
	static void PrintError(const WCHAR* fmt, ...);
	static void PrintDebugSeperator();

private:
	static void log(const WCHAR* a_str);
	static std::wstring dateLog();
	static std::wstring LogDirectory();
	static std::wstring LogFile();
};