#include "Krok3ngine.h"

namespace Engine {
	KEngine g_krok3ngine;

	void SetMode(EngineMode a_engineMode) {
		g_krok3ngine.SetEngineMode(a_engineMode);
	}

	EngineMode GetMode() {
		return g_krok3ngine.GetEngineMode();
	}

	std::wstring EngineModeString(){
		switch (GetMode())
		{
		case EngineMode::DEBUG: return L"Debug";
		case EngineMode::RELEASE: return L"Release";
		case EngineMode::EDITOR: return L"Editor";
		case EngineMode::SERVER: return L"Server";
		default:
			return L"None";
		}
	}
}

KEngine::KEngine() {
#ifdef _DEBUG
	m_engineMode = Engine::EngineMode::DEBUG;
#else
	m_engineMode = EngineMode::RELEASE;
#endif // _DEBUG
}

KEngine::~KEngine() {
}

Engine::EngineMode KEngine::GetEngineMode() {
	return m_engineMode;
}

void KEngine::SetEngineMode(Engine::EngineMode a_enginemode) {
	m_engineMode = a_enginemode;
}
