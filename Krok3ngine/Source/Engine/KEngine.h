#pragma once

class KROK3NGINE_API KEngine;

namespace Engine {
	enum EngineMode : INT {
		NONE,
		DEBUG,
		RELEASE,
		EDITOR,
		SERVER,
	};

	extern KEngine g_krok3ngine;

	VOID KROK3NGINE_API SetMode(EngineMode p_engineMode);
	EngineMode KROK3NGINE_API GetMode();
	std::wstring KROK3NGINE_API EngineModeString();
}

using namespace Engine;
class KROK3NGINE_API KEngine {
public:
	KEngine();
	~KEngine();

public:
	EngineMode GetEngineMode();
	VOID SetEngineMode(EngineMode p_enginemode);

private:
	EngineMode m_engineMode;
};