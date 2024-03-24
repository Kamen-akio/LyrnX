#pragma once
#include "../core/impl/gdipDoubleBuffer.h"
#include "frame.h"

#ifndef _CONTROL_WINDOW_H_
#define _CONTROL_WINDOW_H_

namespace Control {
class Window : public Frame {
 public:
  Window(HWND);
  ~Window();

 protected:
  HWND m_hWindow;
  Gdiplus::CDoubleBuffer* m_pSwapChain;
};
}  // namespace Control

#endif  // !_CONTROL_WINDOW_H_
