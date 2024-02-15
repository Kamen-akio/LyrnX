#pragma once

#define GDIPVER 0x0110

#include "common.h"
#include <Windows.h>
#include <windowsx.h>
#include <gdiplus.h>
#include "../extension/gdip_doubleBuffer.h"
#include "../extension/gdip_graphicsPath.h"

#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Msimg32.lib")

using namespace Gdiplus;