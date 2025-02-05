#pragma once

#include "Core/CoreDefinitions.h"

#include "Common/Exceptions.h"
#include "Common/Logger.h"
#include "Common/Time.h"
#include "Common/Colors.h"
#include "Common/ReadFile.h"

#include "Core/PerGameSettings.h"

#include "Engine/KEngineObject.h"

#ifdef WIN32
	#include "Platform/Win32/Win32Utils.h"
	#include "Platform/Win32/SubObject.h"
	#include "Platform/Win32/Caption.h"
	#include "Platform/Win32/Window.h"
	#include "Platform/Win32/IApplication.h"
#endif // Win32

#include "DirectX/DirectX.h"
#include "Engine/Essentials/Transform.h"
#include "Engine/Graphics/Camera.h"
#include "DirectX/Renderer.h"
#include "Engine/Graphics/RenderObject.h"

#include "Engine/KEngine.h"