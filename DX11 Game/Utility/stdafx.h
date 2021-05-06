#pragma once
#ifndef STDAFX_H
#define STDAFX_H

/// <summary>
/// Precompiled header. If a class is using a precompiled header, this class must be the first thing included in the cpp file.
/// Used to reduce compilation times throughout program by precompiling commonly used libraries/classes.
/// Cherno: Precompiled Headers in C++ - https://www.youtube.com/watch?v=eSI4wctZUto
/// </summary>

#include <memory>
#include <algorithm>

// Data Structures
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>

// Windows API
#include <Windows.h>
#include <d3d11_1.h>
#include <wrl/client.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>

using namespace DirectX;

// Classes
#include "Timer.h"
#include "ErrorLogger.h"
#include "EventSystem/EventSystem.h"

#endif