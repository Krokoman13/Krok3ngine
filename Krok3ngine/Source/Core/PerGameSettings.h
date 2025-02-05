#pragma once

class KROK3NGINE_API PerGameSettings {
private:
	static PerGameSettings* m_inst;
	static PerGameSettings* Instance() { return m_inst; }

public:
	PerGameSettings();
	~PerGameSettings();

private:
	WCHAR m_gameName[MAX_NAME_STRING];
	WCHAR m_shortName[MAX_NAME_STRING];
	HICON m_mainIcon;
	WCHAR m_bootTime[MAX_NAME_STRING];
	WCHAR m_splashURL[MAX_NAME_STRING];

public:
	static WCHAR* GameName() { return m_inst->m_gameName; }
	static void SetGameName(UINT id) { LoadString(HInstance(), id, m_inst->m_gameName, MAX_NAME_STRING); }

	static WCHAR* ShortName() { return m_inst->m_shortName; }
	static void SetShortName(UINT id) { LoadString(HInstance(), id, m_inst->m_shortName, MAX_NAME_STRING); }

	static HICON MainIcon() { return m_inst->m_mainIcon; }
	static void SetMainIcon(UINT id) { LoadIcon(HInstance(), MAKEINTRESOURCE(id)); }

	static WCHAR* BootTime() { return m_inst->m_bootTime; }

	static WCHAR* SplashURL() { return m_inst->m_splashURL; }
	static void SetSplashURL(UINT id) { LoadString(HInstance(), id, m_inst->m_splashURL, MAX_NAME_STRING); }
};