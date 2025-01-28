#pragma once

#include "Core/CoreDefinitions.h"
#include "DirectX/DirectX.h"

#include "Engine/KEngine.h"

#include "Common/Logger.h"
#include "Common/Time.h"
#include "Core/PerGameSettings.h"

#ifdef WIN32
	#include "Platform/Win32/Win32Utils.h"
	#include "Platform/Win32/SubObject.h"
	#include "Platform/Win32/Caption.h"
	#include "Platform/Win32/Window.h"
	#include "Platform/Win32/IApplication.h"
#endif // Win32
