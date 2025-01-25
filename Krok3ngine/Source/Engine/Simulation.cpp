#include "Krok3ngine.h"
#include "Simulation.h"

#include "SplashScreen.h"

namespace Krok3ngine {
	Simulation::Simulation() 
		: Win32::Window(L"MainApplication", Win32::RESIZABLE) {
	}

	Simulation::~Simulation() {
	}

	VOID Simulation::PreInitialize() {
		Logger::PrintDebugSeperator();
		Logger::PrintLog(L"Application Starting...");
		Logger::PrintLog(L"Game Name: %s", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s", Time::GetDateTimeString().c_str());
		Logger::PrintLog(L"Engine Mode: %s", Engine::EngineModeString().c_str());
		Logger::PrintDebugSeperator();

		SplashScreen::Open();
		SplashScreen::AddMessage(L"Starting Application...");

		Win32::Window::RegisterNewClass();
		Win32::Window::Initialize();
	}

	LRESULT Simulation::MessageHandler(HWND a_hWnd, UINT a_message, WPARAM a_wparam, LPARAM a_lparam) {
		switch (a_message) {
		}

		return Window::MessageHandler(a_hWnd, a_message, a_wparam, a_lparam);
	}
}
