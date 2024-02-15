#pragma once
#include "../dependencies/render.h"
#include "../dependencies/render_template.h"
#include "../extension/win_blurHelper.h"
#include "../extension/win_registry.h"
#include "../utils/environment.h"
#include "../utils/node.h"

#ifndef _WINDOW_RENDER_H_
#define _WINDOW_RENDER_H_

typedef struct __tagWindowProp {
  HWND handle;
  SIZE size;
  POINT position;

  // eee

  bool IsFocused;
} WindowProp, WndProp;

namespace WindowRender {
bool Processed();
LRESULT __stdcall WindowProcess(HWND hWnd,
                                UINT uMsg,
                                WPARAM wParam,
                                LPARAM lParam);
}  // namespace WindowRender

#endif  // _WINDOW_RENDER_H_