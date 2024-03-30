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
  static bool bFirstInit = false;
  if (not bFirstInit) {
    InitContext initContext{};
    initContext.RenderTarget = this;

    CallObjectProc(WM_CREATE, wParam, (LPARAM)&initContext);
    // EventPrcessor(WM_CREATE, 0, 0);

    bFirstInit = true;
  }

  if (uMsg == WM_SIZE) {
    m_swapChain->ResizeBuffer(Gdiplus::SizeF(LOWORD(lParam), HIWORD(lParam)));
    m_rcObject.Width = m_swapChain->GetBufferSize().Width;
    m_rcObject.Height = m_swapChain->GetBufferSize().Height;
  }

  if (WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST || uMsg == WM_MOUSELEAVE) {
    CallObjectProc(uMsg, wParam, lParam);
    return EventPrcessor(WM_PAINT, 0, 0);
  }

  if (uMsg == WM_PAINT) {
    bool result = false;
    RenderContext* renderContext{};

    renderContext = RenderBegin();

    Graphics* srcGrap = renderContext->graphics;
    srcGrap->Clear(Color::Gray);

    result |= _OnPaint(*srcGrap) or
              CallObjectProc(uMsg, wParam, (LPARAM)(void*)renderContext);

    if (not result) {
      return true;
    }

    RenderEnd(renderContext);
    return true;
  }

  return ((object*)this)->EventPrcessor(uMsg, wParam, lParam);
}

Control::RenderContext* Control::screen::RenderBegin(RectF rcTarget) {
  auto presentRect = new RectF;
  auto srcGrap = new Graphics(m_swapChain->GetDC());
  return new RenderContext{this, srcGrap, rcTarget, presentRect};
}

void Control::screen::RenderEnd(RenderContext* context) {
  if (m_swapChain->m_isResized) {
    m_swapChain->InvalidateRect(m_rcObject);
    m_swapChain->m_isResized = false;
  }

  m_swapChain->InvalidateRect(*context->presentRect);
  m_swapChain->Present();

  delete context->graphics;
  delete context->presentRect;
  delete context;
}
