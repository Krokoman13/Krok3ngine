#include "Krok3ngine.h"
#include "Simulation.h"

#include "SplashScreen.h"

namespace Krok3ngine {
	Simulation::Simulation() {
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
	}
}
