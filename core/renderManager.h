#pragma once
#include <Windows.h>
#include <Windowsx.h>

#include <vector>

#include "./impl/gdipExt.h"
#include "./impl/multiwayTree.h"

#ifndef _CORE_RENDERMANAGER_H_
#define _CORE_RENDERMANAGER_H_

namespace Core {
class CRenderManager : public Gdiplus::DoubleBuffer {
 public:
  CRenderManager(HWND hWnd);

 public:
  void ApplyOffset(Gdiplus::PointF pos = {0, 0});

  void InvalidateRect(Gdiplus::RectF rect);
  void PresentAll();

 private:
  std::vector<Gdiplus::RectF> m_layoutRectQueue;
  Gdiplus::PointF m_ptOffset;
};
}  // namespace Core

#endif  // !_CORE_RENDERMANAGER_H_
