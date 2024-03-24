#include "gdipDoubleBuffer.h"

using Gdiplus::SizeF;

static HBITMAP CreateVirtualBitmap(HDC hDC, int width, int height) {
  BITMAPINFO bitmapInfo{};
  bitmapInfo.bmiHeader.biBitCount = 32;
  bitmapInfo.bmiHeader.biCompression = 0;
  bitmapInfo.bmiHeader.biPlanes = 1;
  bitmapInfo.bmiHeader.biSize = 40;
  bitmapInfo.bmiHeader.biWidth = width;
  bitmapInfo.bmiHeader.biHeight = height;

  return CreateDIBSection(hDC, &bitmapInfo, DIB_RGB_COLORS, nullptr, NULL, 0);
}

Gdiplus::CDoubleBuffer::CDoubleBuffer(HWND hWnd) : m_hWindow(hWnd) {
  RECT rcWnd{};

  auto a = GetWindowRect(hWnd, &rcWnd);
  m_szWindow = SizeF(rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

  m_hVirtualDC = CreateCompatibleDC(NULL);
  m_hVirtualBitmap =
      CreateVirtualBitmap(m_hVirtualDC, m_szWindow.Width, m_szWindow.Height);
  m_hLastVirtualBitmap = (HBITMAP)SelectObject(m_hVirtualDC, m_hVirtualBitmap);
}

Gdiplus::CDoubleBuffer::~CDoubleBuffer() {
  if (m_hLastVirtualBitmap) {
    SelectObject(m_hVirtualDC, m_hLastVirtualBitmap);
    DeleteObject(m_hVirtualBitmap);
  }
  DeleteDC(m_hVirtualDC);
}

void Gdiplus::CDoubleBuffer::Present(RectF* rect) {
  HDC hTargetDC = ::GetDC(m_hWindow);

  if (rect != nullptr) {
    BitBlt(hTargetDC, rect->X, rect->Y, rect->Width, rect->Height, m_hVirtualDC,
           rect->X, rect->Y, SRCCOPY);
  } else {
    BitBlt(hTargetDC, 0, 0, m_szWindow.Width, m_szWindow.Height, m_hVirtualDC,
           0, 0, SRCCOPY);
  }

  ReleaseDC(m_hWindow, hTargetDC);
}

HDC Gdiplus::CDoubleBuffer::GetDC() const { return m_hVirtualDC; }

SizeF Gdiplus::CDoubleBuffer::GetBufferSize() const { return m_szWindow; }

HBITMAP Gdiplus::CDoubleBuffer::GetBufferHandle() const {
  return m_hVirtualBitmap;
}

void Gdiplus::CDoubleBuffer::ResizeBuffer(SizeF szWnd) {
  SelectObject(m_hVirtualDC, m_hLastVirtualBitmap);
  DeleteObject(m_hVirtualBitmap);

  m_hVirtualBitmap =
      CreateVirtualBitmap(m_hVirtualDC, szWnd.Width, szWnd.Height);
  m_hLastVirtualBitmap = (HBITMAP)SelectObject(m_hVirtualDC, m_hVirtualBitmap);

  m_szWindow.Width = szWnd.Width;
  m_szWindow.Height = szWnd.Height;
  // this->Present();
}
