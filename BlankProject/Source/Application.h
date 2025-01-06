#pragma once

#include "Engine/Simulation.h"

class Application : public Krok3ngine::Simulation {
public:
	Application();
	~Application();

public:
	VOID SetupPerGameSettings();
	VOID Initialize();
	VOID Update();
};