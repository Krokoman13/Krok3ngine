#pragma once

#include "Platform/Win32/IApplication.h"

namespace Krok3ngine {
	class KROK3NGINE_API Simulation : public Win32::IApplication{
	public:
		Simulation();
		~Simulation();

	public:
		virtual VOID PreInitialize() override;
	};
}