#include "Krok3ngine.h"
#include "Simulation.h"

#include "SplashScreen.h"

namespace Krok3ngine {
	Simulation::Simulation() : 
		m_window(L"Application", Win32::RESIZABLE, NULL) {
	}

	Simulation::~Simulation() {
	}

	void Simulation::baseInitialize() {
		Logger::PrintDebugSeperator();
		Logger::PrintLog(L"Application Starting...");
		Logger::PrintLog(L"Game Name: %s", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s", Time::GetDateTimeString().c_str());
		Logger::PrintLog(L"Engine Mode: %s", Engine::EngineModeString().c_str());
		Logger::PrintDebugSeperator();

		SplashScreen::Open();
		SplashScreen::AddMessage(L"Starting Application...");
	}

	void Simulation::baseLoad() {
		m_window.Initialize();
		m_renderer.Initialize(&m_window);
	}

	void Simulation::baseUpdate() {
		m_renderer.Render();
	}

}
