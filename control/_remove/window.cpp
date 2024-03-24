#include "window.h"

Control::Window::Window(HWND hWnd) {
  RECT rcWnd{};
  GetWindowRect(m_hWindow, &rcWnd);

  m_rcElement = {rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left,
                 rcWnd.bottom - rcWnd.top};

  m_pSwapChain = new CDoubleBuffer(hWnd);
}

Control::Window::~Window() {
  if (m_pSwapChain != nullptr) {
    delete m_pSwapChain;
    m_pSwapChain = nullptr;
  }
}
