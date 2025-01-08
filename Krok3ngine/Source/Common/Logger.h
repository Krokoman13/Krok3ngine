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
	static VOID PrintLog(const WCHAR* fmt, ...);
	static VOID PrintWarning(const WCHAR* fmt, ...);
	static VOID PrintError(const WCHAR* fmt, ...);
	static VOID PrintDebugSeperator();

private:
	static VOID log(const WCHAR* a_str);
	static std::wstring dateLog();
	static std::wstring LogDirectory();
	static std::wstring LogFile();
};