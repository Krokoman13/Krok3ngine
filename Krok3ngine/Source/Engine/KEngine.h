#pragma once

class KROK3NGINE_API KEngine;

namespace Engine {
	enum EngineMode : int {
		NONE,
		DEBUG,
		RELEASE,
		EDITOR,
		SERVER,
	};

	extern KEngine g_krok3ngine;

	void KROK3NGINE_API SetMode(EngineMode a_engineMode);
	EngineMode KROK3NGINE_API GetMode();
	std::wstring KROK3NGINE_API EngineModeString();
}

class KROK3NGINE_API KEngine {
public:
	KEngine();
	~KEngine();

public:
	Engine::EngineMode GetEngineMode();
	void SetEngineMode(Engine::EngineMode a_enginemode);

private:
	Engine::EngineMode m_engineMode;
};