#include "renderManager.h"

Core::CRenderManager::CRenderManager(HWND hWnd) : CDoubleBuffer(hWnd) {
  m_layoutRectQueue = vector<RectF>();
}

void Core::CRenderManager::InvalidateRect(RectF rect) {
  m_layoutRectQueue.push_back(rect);
}

void Core::CRenderManager::Present() {
  for (auto &rcLayout : m_layoutRectQueue) {
    ((CDoubleBuffer *)this)->Present(&rcLayout);
  }

  m_layoutRectQueue.clear();
}
