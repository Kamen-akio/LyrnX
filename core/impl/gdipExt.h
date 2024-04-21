#pragma once
#include <Windows.h>
#include <gdiplus.h>

#include <string>

#pragma comment(lib, "gdiplus.lib")

#ifndef _IMPL_GDIPLUS_DOUBLEBUFFER_H_
#define _IMPL_GDIPLUS_DOUBLEBUFFER_H_

namespace Gdiplus {

Rect RectF2Rect(RectF rc);

void CreateRoundedPath(GraphicsPath& path, Rect rect, int radius);
void CreateRoundedPath(GraphicsPath& path, RectF rect, FLOAT radius);

void CreateBezierRoundedPath(GraphicsPath& path, Size rect, int radius);
void CreateBezierRoundedPath(GraphicsPath& path, SizeF rect, FLOAT radius);

Color GetColorFromStringA(std::string);
Color GetColorFromStringW(std::wstring);

class DoubleBuffer {
 public:
  DoubleBuffer(HWND hWnd);
  ~DoubleBuffer();

  void Present();
  void Present(Rect rect);
  void Present(Rect rect, PointF offset);
  void Present(WPARAM wParam);

  HDC GetDC() const;
  HWND GetWindowHandle() const;
  HBITMAP GetBufferHandle() const;

  void ResizeBuffer(Size szWnd);
  Size GetBufferSize() const;

 public:
  bool m_isResized;

 protected:
  HWND m_hWindow;
  Size m_szWindow;
  HDC m_hVirtualDC;
  HBITMAP m_hVirtualBitmap;
  HBITMAP m_hLastVirtualBitmap;
};
}  // namespace Gdiplus

#endif  // !_IMPL_GDIPLUS_DOUBLEBUFFER_H_
