#include "BlankProject.h"

#include "Engine/Simulation.h"
#include "Engine/Graphics/BasicMesh.h"
#include "Platform/Win32/WinEntry.h"

class BlankProject : public Krok3ngine::Simulation {
public:
	BlankProject() { 
		mesh = std::make_unique<Engine::BasicMesh>("Content/Models/dragon/dragon.obj");
		AddRenderObject(mesh.get());
	};
	~BlankProject() {};

public:
	void SetupPerGameSettings();

protected:
	virtual void load() override{
		Engine::Camera::GetMainCamera()->Move(0.f, -20.f, 35.f);
		//Engine::Camera::GetMainCamera()->Move(0.f, 0.f, 1.f);
		mesh->Rotate(1.f, 0.f, 0.f, -DirectX::XM_PI / 2.f);
	}

	virtual void update() override {
		Engine::Camera::GetMainCamera()->Move(0.f, -0.01f, 0.01f);
		mesh->Rotate(0.f, 1.f, 0.f, 0.01f);
	}

private:
	std::unique_ptr<Engine::BasicMesh> mesh;
};

ENTRYAPP(BlankProject)

void BlankProject::SetupPerGameSettings() {
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
	//PerGameSettings::SetSplashURL(IDS_SPLASHURL);
}