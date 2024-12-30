#include "pch.h"
#include "Application.h"
#include "Platform/WIN32/WinEntry.h"

ENTRYAPP(Application)

Application::Application() {
}

Application::~Application() {
}

VOID Application::SetupPerGameSettings() {
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME); 
	PerGameSettings::SetMainIcon(IDI_MAINICON);
}

VOID Application::Initialize() {
	Logger::PrintDebugSeperator();
	Logger::PrintLog(L"Application Starting...");
	Logger::PrintLog(L"Game Name: %s", PerGameSettings::GameName());
	Logger::PrintLog(L"Boot Time: %s", Time::GetDateTimeString().c_str());
	Logger::PrintDebugSeperator();

	Logger::PrintLog(L"Test");
	Logger::PrintWarning(L"Test");
	Logger::PrintLog(L"Test");
	Logger::PrintError(L"Test");
	Logger::PrintLog(L"Test");
}

VOID Application::Update(){
	Logger::PrintLog(L"Loop");
}
