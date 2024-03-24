#include "screen.h"

Control::screen::~screen() {
  if (m_swapChain != nullptr) {
    delete m_swapChain;
    m_swapChain = nullptr;
  }
}

void Control::screen::Create(HWND hWnd) {
  m_swapChain = new Core::CRenderManager(hWnd);
  m_bEnable = true;
  m_bVisible = true;
  m_rcObject = RectF({0, 0}, m_swapChain->GetBufferSize());
}

bool Control::screen::EventPrcessor(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (uMsg == WM_SIZE) {
    m_swapChain->ResizeBuffer(Gdiplus::SizeF(LOWORD(lParam), HIWORD(lParam)));
    m_rcObject.Width = m_swapChain->GetBufferSize().Width;
    m_rcObject.Height = m_swapChain->GetBufferSize().Height;
  }

  if ((WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST) or uMsg == WM_PAINT) {
    bool result = false;
    Graphics srcGrap(m_swapChain->GetDC());
    srcGrap.Clear(Color::DarkMagenta);

    _OnPaint(srcGrap);
    if (uMsg == WM_PAINT) {
      lParam = (LPARAM)(void*)&srcGrap;
    } else {
      result = EventPrcessor(WM_PAINT, 0, 0);
    }

    result = CallObjectProc(uMsg, wParam, lParam);
    m_swapChain->InvalidateRect(m_rcObject);
    m_swapChain->Present();

    return result;
  }

  return ((object*)this)->EventPrcessor(uMsg, wParam, lParam);
}
