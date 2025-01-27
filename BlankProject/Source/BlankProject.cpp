#include "BlankProject.h"

#include "Engine/Simulation.h"
#include "Platform/Win32/WinEntry.h"

#include "Sample.h"


class BlankProject : public Krok3ngine::Simulation {
public:
	BlankProject() {};
	~BlankProject() {};

public:
	VOID SetupPerGameSettings();
	VOID Initialize();
	VOID Update();

private:
	Sample triangle;
};

ENTRYAPP(BlankProject)

VOID BlankProject::SetupPerGameSettings() {
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
	//PerGameSettings::SetSplashURL(IDS_SPLASHURL);
}

VOID BlankProject::Initialize() {
	triangle.Initialize(GetHandle(), GetSize().cx, GetSize().cy);
}

VOID BlankProject::Update() {
	triangle.Tick();
}