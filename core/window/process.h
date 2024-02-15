#pragma once
#include "../dependencies/process.h"

namespace WindowProcess {
bool Processed();
LRESULT __stdcall WindowProcess(HWND hWnd,
                                UINT uMsg,
                                WPARAM wParam,
                                LPARAM lParam);
};  // namespace WindowProcess