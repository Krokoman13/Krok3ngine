#include "Krok3ngine.h"

PerGameSettings* PerGameSettings::m_inst;

PerGameSettings::PerGameSettings() {
	m_inst = this;

	wcscpy_s(m_inst->m_GameName, L"undefined");
	wcscpy_s(m_inst->m_ShortName, L"undefined");
	wcscpy_s(m_inst->m_BootTime, Time::GetDateTimeString(TRUE).c_str());
}

PerGameSettings::~PerGameSettings() {
}