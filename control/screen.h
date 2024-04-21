#pragma once
#include "Frame.h"
#ifndef _COMPONENT_SCREEN_H_
#define _COMPONENT_SCREEN_H_

namespace Component {
class Screen : public Frame {
 public:
  Screen();
  ~Screen();
  void Bind(HWND hWnd);

 public:
  void SetChild(ControlBase*);
  LRESULT CallChildrenProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  LRESULT MessageProcess(UINT uMsg, WPARAM wParam, LPARAM lParam);

  bool OnPaint(Graphics* presentGraphics);

 protected:
  HWND m_presentWnd;
};
}  // namespace Component

#endif
