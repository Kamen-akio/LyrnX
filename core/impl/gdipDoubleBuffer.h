#pragma once
#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

#ifndef _IMPL_GDIPLUS_DOUBLEBUFFER_H_
#define _IMPL_GDIPLUS_DOUBLEBUFFER_H_

namespace Gdiplus {
class CDoubleBuffer {
 public:
  CDoubleBuffer(HWND hWnd);
  ~CDoubleBuffer();

  void Present(RectF* rect = nullptr);

  HDC GetDC() const;
  HBITMAP GetBufferHandle() const;

  void ResizeBuffer(SizeF szWnd);
  SizeF GetBufferSize() const;

 protected:
  HWND m_hWindow;
  SizeF m_szWindow;
  HDC m_hVirtualDC;
  HBITMAP m_hVirtualBitmap;
  HBITMAP m_hLastVirtualBitmap;
};
}  // namespace Gdiplus

#endif  // !_IMPL_GDIPLUS_DOUBLEBUFFER_H_
