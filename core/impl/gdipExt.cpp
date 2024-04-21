#include "gdipExt.h"

#include <algorithm>

using Gdiplus::Color;
using Gdiplus::Size;
using Gdiplus::SizeF;

// ==========
//
// Gdiplus::Color Ext Impl.
//
// ==========

static long hex2dec(std::string str) {
  char in[1024]{0};
  long out = 0;
  strcpy_s(in, str.c_str());

  char* pin = in;

  while (*pin != 0) {
    out <<= 4;
    out += (*pin < 'A') ? *pin & 0xF : (*pin & 0x7) + 9;
    pin++;
  }
  return out;
}

Gdiplus::Rect Gdiplus::RectF2Rect(RectF rc) {
  return Rect((int)rc.X, (int)rc.Y, (int)rc.Width, (int)rc.Height);
}

void Gdiplus::CreateRoundedPath(GraphicsPath& path, Rect rect, int radius) {
  radius = radius * 2;

  path.AddArc(rect.GetRight() - radius, rect.GetTop(), radius, radius, 270.f,
              90.f);

  path.AddArc(rect.GetRight() - radius, rect.GetBottom() - radius, radius,
              radius, 0.f, 90.f);

  path.AddArc(rect.GetLeft(), rect.GetBottom() - radius, radius, radius, 90.f,
              90.f);

  path.AddArc(rect.GetLeft(), rect.GetTop(), radius, radius, 180.f, 90.f);

  path.CloseFigure();
}

void Gdiplus::CreateRoundedPath(GraphicsPath& path, RectF rect, FLOAT radius) {
  radius = radius * 2;

  path.AddArc(rect.GetRight() - radius, rect.GetTop(), radius, radius, 270.f,
              90.f);

  path.AddArc(rect.GetRight() - radius, rect.GetBottom() - radius, radius,
              radius, 0.f, 90.f);

  path.AddArc(rect.GetLeft(), rect.GetBottom() - radius, radius, radius, 90.f,
              90.f);

  path.AddArc(rect.GetLeft(), rect.GetTop(), radius, radius, 180.f, 90.f);

  path.CloseFigure();
}

void Gdiplus::CreateBezierRoundedPath(GraphicsPath& path, Size rect,
                                      int radius) {
  radius = radius * 2;

  path.AddBezier(0, radius, 0, 0, 0, 0, radius, 0);

  path.AddBezier(rect.Width - radius, 0, rect.Width, 0, rect.Width, 0,
                 rect.Width, radius);

  path.AddBezier(rect.Width, rect.Height - radius, rect.Width, rect.Height,
                 rect.Width, rect.Height, rect.Width - radius, rect.Height);

  path.AddBezier(radius, rect.Height, 0, rect.Height, 0, rect.Height, 0,
                 rect.Height - radius);

  path.CloseFigure();
}

void Gdiplus::CreateBezierRoundedPath(GraphicsPath& path, SizeF rect,
                                      FLOAT radius) {
  radius = radius * 2;

  path.AddBezier(0.f, radius, 0.f, 0.f, 0.f, 0.f, radius, 0.f);

  path.AddBezier(rect.Width - radius, 0.f, rect.Width, 0.f, rect.Width, 0.f,
                 rect.Width, radius);

  path.AddBezier(rect.Width, rect.Height - radius, rect.Width, rect.Height,
                 rect.Width, rect.Height, rect.Width - radius, rect.Height);

  path.AddBezier(radius, rect.Height, 0.f, rect.Height, 0.f, rect.Height, 0.f,
                 rect.Height - radius);

  path.CloseFigure();
}

// Using ARGB format.
Color Gdiplus::GetColorFromStringA(std::string color) {
  if (color.length() % 2) {
    color = color.substr(1, color.length() - 1);
  }

  if (color.length() < 8) {
    color = "FF" + color;
  }

  auto data = hex2dec(color);
  return Color(data >> 24 & 0xFF, data >> 16 & 0xFF, data >> 8 & 0xFF,
               data & 0xFF);
}

Color Gdiplus::GetColorFromStringW(std::wstring wcolor) {
  std::string color(wcolor.length(), 0);
  std::transform(wcolor.begin(), wcolor.end(), color.begin(),
                 [](wchar_t c) { return (char)c; });

  return GetColorFromStringA(color);
}

// ==========
//
// Gdiplus::DoubleBuffer Impl.
//
// ==========

static HBITMAP CreateVirtualBitmap(HDC hDC, int width, int height) {
  void* ppvBits{};
  BITMAPINFO bitmapInfo{};
  bitmapInfo.bmiHeader.biBitCount = 32;
  bitmapInfo.bmiHeader.biCompression = 0;
  bitmapInfo.bmiHeader.biPlanes = 1;
  bitmapInfo.bmiHeader.biSize = 40;
  bitmapInfo.bmiHeader.biWidth = width;
  bitmapInfo.bmiHeader.biHeight = height;

  return CreateDIBSection(hDC, &bitmapInfo, DIB_RGB_COLORS, &ppvBits, NULL, 0);
}

Gdiplus::DoubleBuffer::DoubleBuffer(HWND hWnd)
    : m_hWindow(hWnd), m_isResized(false) {
  RECT rcWnd{};

  GetWindowRect(hWnd, &rcWnd);
  m_szWindow = Size(rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

  m_hVirtualDC = CreateCompatibleDC(NULL);
  m_hVirtualBitmap =
      CreateVirtualBitmap(m_hVirtualDC, m_szWindow.Width, m_szWindow.Height);
  m_hLastVirtualBitmap = (HBITMAP)SelectObject(m_hVirtualDC, m_hVirtualBitmap);
}

Gdiplus::DoubleBuffer::~DoubleBuffer() {
  if (m_hLastVirtualBitmap) {
    SelectObject(m_hVirtualDC, m_hLastVirtualBitmap);
    DeleteObject(m_hVirtualBitmap);
  }
  DeleteDC(m_hVirtualDC);
}

void Gdiplus::DoubleBuffer::Present() {
  HDC hTargetDC = ::GetDC(m_hWindow);

  BitBlt(hTargetDC, 0, 0, m_szWindow.Width, m_szWindow.Height, m_hVirtualDC, 0,
         0, SRCCOPY);

  ReleaseDC(m_hWindow, hTargetDC);
}

void Gdiplus::DoubleBuffer::Present(Rect rect) {
  HDC hTargetDC = ::GetDC(m_hWindow);

  BitBlt(hTargetDC, rect.X, rect.Y, rect.Width, rect.Height, m_hVirtualDC,
         rect.X, rect.Y, SRCCOPY);

  ReleaseDC(m_hWindow, hTargetDC);
}

void Gdiplus::DoubleBuffer::Present(Rect rect, PointF offset) {
  HDC hTargetDC = ::GetDC(m_hWindow);

  BitBlt(hTargetDC, rect.X, rect.Y, rect.Width, rect.Height, m_hVirtualDC,
         rect.X - (int)offset.X, rect.Y - (int)offset.Y, SRCCOPY);

  ReleaseDC(m_hWindow, hTargetDC);
}

void Gdiplus::DoubleBuffer::Present(WPARAM wParam) {
  HRGN presentRect{};
  presentRect = (HRGN)wParam;

  if (wParam == NULLREGION) {
    RECT wndRect{};
    GetWindowRect(m_hWindow, &wndRect);

    presentRect =
        CreateRectRgn(wndRect.left, wndRect.top, wndRect.right, wndRect.bottom);
  }

  HDC hTargetDC = ::GetDCEx(m_hWindow, presentRect,
                            DCX_WINDOW | DCX_INTERSECTRGN | DCX_CACHE);

  auto ret = GetLastError();

  BitBlt(hTargetDC, 0, 0, m_szWindow.Width, m_szWindow.Height, m_hVirtualDC, 0,
         0, SRCCOPY);

  ReleaseDC(m_hWindow, hTargetDC);
}

HDC Gdiplus::DoubleBuffer::GetDC() const { return m_hVirtualDC; }

HWND Gdiplus::DoubleBuffer::GetWindowHandle() const { return m_hWindow; }

Size Gdiplus::DoubleBuffer::GetBufferSize() const { return m_szWindow; }

HBITMAP Gdiplus::DoubleBuffer::GetBufferHandle() const {
  return m_hVirtualBitmap;
}

void Gdiplus::DoubleBuffer::ResizeBuffer(Size szWnd) {
  SelectObject(m_hVirtualDC, m_hLastVirtualBitmap);
  DeleteObject(m_hVirtualBitmap);

  m_hVirtualBitmap =
      CreateVirtualBitmap(m_hVirtualDC, szWnd.Width, szWnd.Height);
  m_hLastVirtualBitmap = (HBITMAP)SelectObject(m_hVirtualDC, m_hVirtualBitmap);

  m_szWindow.Width = szWnd.Width;
  m_szWindow.Height = szWnd.Height;

  m_isResized = true;
}
