#pragma once

#include "./impl/gdipExt.h"
#include "./impl/dwmHelper.h"
#include "./impl/multiwayTree.h"
#include "./renderManager.h"

#include "../control/__index.h"

#include <exception>
#include <Windows.h>

#ifndef _CORE_MAIN_H_
#define _CORE_MAIN_H_

namespace Core {
void Init(HINSTANCE hInstance, int nCmdShow, HWND& hWnd);
void Destroy();
LRESULT GlobalMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam,
                             LPARAM lParam);
}

#endif  // !_CORE_MAIN_H_
