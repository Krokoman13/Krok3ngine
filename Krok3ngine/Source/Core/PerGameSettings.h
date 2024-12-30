#pragma once

class KROK3NGINE_API PerGameSettings {
private:
	/* Singleton reference to static class */
	static PerGameSettings* m_inst;
	static PerGameSettings* Instance() { return m_inst; }

public:
	PerGameSettings();
	~PerGameSettings();

private:
	/* Per Game Private Variables */
	WCHAR m_GameName[MAX_NAME_STRING];
	WCHAR m_ShortName[MAX_NAME_STRING];
	HICON m_MainIcon;
	WCHAR m_BootTime[MAX_NAME_STRING];

public:
	/* Access Getters and Setters */
	static WCHAR* GameName() { return m_inst->m_GameName; }
	static VOID SetGameName(UINT id) { LoadString(HInstance(), id, m_inst->m_GameName, MAX_NAME_STRING); }

	static WCHAR* ShortName() { return m_inst->m_ShortName; }
	static VOID SetShortName(UINT id) { LoadString(HInstance(), id, m_inst->m_ShortName, MAX_NAME_STRING); }

	static HICON MainIcon() { return m_inst->m_MainIcon; }
	static VOID SetMainIcon(UINT id) { LoadIcon(HInstance(), MAKEINTRESOURCE(id)); }

	static WCHAR* BootTime() { return m_inst->m_BootTime; }
};