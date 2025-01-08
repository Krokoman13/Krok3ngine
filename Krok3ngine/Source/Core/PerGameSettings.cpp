#include "Krok3ngine.h"

PerGameSettings* PerGameSettings::m_inst;

PerGameSettings::PerGameSettings() {
	m_inst = this;

	wcscpy_s(m_inst->m_gameName, L"undefined");
	wcscpy_s(m_inst->m_shortName, L"undefined"); 
	wcscpy_s(m_inst->m_bootTime, Time::GetDateTimeString(TRUE).c_str());
	wcscpy_s(m_inst->m_splashURL, L"..\\Krok3ngine\\Content\\Images\\Krokso.bmp");
}

PerGameSettings::~PerGameSettings() {
}