#pragma once
#include <Windows.h>
#include <Windowsx.h>

#include <vector>

#include "./impl/gdipDoubleBuffer.h"
#include "./impl/multiwayTree.h"

using Gdiplus::RectF;
using std::vector;

#ifndef _CORE_RENDERMANAGER_H_
#define _CORE_RENDERMANAGER_H_

namespace Core {
class CRenderManager : public Gdiplus::CDoubleBuffer {
 public:
  CRenderManager(HWND hWnd);
  void InvalidateRect(RectF rect);
  void Present();

 private:
  vector<RectF> m_layoutRectQueue;
};
}  // namespace Core

#endif  // !_CORE_RENDERMANAGER_H_
