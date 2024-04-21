#include "renderManager.h"

using namespace Gdiplus;
using std::vector;

Core::CRenderManager::CRenderManager(HWND hWnd)
    : DoubleBuffer(hWnd), m_layoutRectQueue({}), m_ptOffset({}) {}

#pragma warning(push)
#pragma warning(disable : 4244)

void Core::CRenderManager::ApplyOffset(PointF pos) { m_ptOffset = PointF(pos); }

void Core::CRenderManager::InvalidateRect(RectF rect) {
  m_layoutRectQueue.push_back(RectF(rect));
  RECT rc{};
  rc.left = floor(rect.GetLeft());
  rc.top = floor(rect.GetTop());
  rc.bottom = ceil(rect.GetBottom());
  rc.right = ceil(rect.GetRight());
  ::InvalidateRect(m_hWindow, &rc, FALSE);
}

#pragma warning(pop)

void Core::CRenderManager::PresentAll() {
  for (auto &rcLayout : m_layoutRectQueue) {
    rcLayout.Offset(m_ptOffset);
    ((DoubleBuffer *)this)->Present(RectF2Rect(rcLayout), m_ptOffset);
  }

  m_layoutRectQueue.clear();
}
