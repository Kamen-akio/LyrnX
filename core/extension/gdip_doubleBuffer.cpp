#pragma once
#include "gdip_doubleBuffer.h"

inline static HBITMAP CreateMemoryBitmap(HDC dc, SIZE szWnd);

Gdiplus::DoubleBuffer::DoubleBuffer(HWND hWnd) {
  _hWnd = hWnd;
  _hVirtualDC = CreateCompatibleDC(NULL);

  FlushByApi();
}

Gdiplus::DoubleBuffer::~DoubleBuffer() {
  if (_hVirtualBitmap) {
    SelectObject(_hVirtualDC, _hLastVirtualBitmap);
    DeleteObject(_hVirtualBitmap);
  }

  DeleteDC(_hVirtualDC);
};

HDC& Gdiplus::DoubleBuffer::GetRenderDC() {
  return _hVirtualDC;
}

void Gdiplus::DoubleBuffer::Flush() {
  HDC hTargetDC = GetDC(_hWnd);
  POINT ptDst{0, 0};

  BLENDFUNCTION blendFunc{};
  blendFunc.BlendOp = AC_SRC_OVER;
  blendFunc.BlendFlags = 0;
  blendFunc.AlphaFormat = AC_SRC_ALPHA;
  blendFunc.SourceConstantAlpha = 255;

  UpdateLayeredWindow(_hWnd, hTargetDC, &_posWnd, &_szWnd, _hVirtualDC, &ptDst,
                      0, &blendFunc, ULW_ALPHA);
  ReleaseDC(_hWnd, hTargetDC);
}

void Gdiplus::DoubleBuffer::FlushPos(POINT posWnd) {
  _posWnd = posWnd;
}

void Gdiplus::DoubleBuffer::FlushSize(SIZE szWnd) {
  if (_hVirtualBitmap) {
    DeleteObject(SelectObject(_hVirtualDC, _hLastVirtualBitmap));
  }

  _szWnd = szWnd;
  _hVirtualBitmap = CreateMemoryBitmap(_hVirtualDC, szWnd);
  _hLastVirtualBitmap = (HBITMAP)SelectObject(_hVirtualDC, _hVirtualBitmap);
}

void Gdiplus::DoubleBuffer::FlushByApi() {
  RECT rcWnd{};
  GetWindowRect(_hWnd, &rcWnd);
  FlushPos({rcWnd.left, rcWnd.top});
  FlushSize({rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top});
}

inline static HBITMAP CreateMemoryBitmap(HDC dc, SIZE szWnd) {
  BITMAPINFO bitmapInfo{};
  bitmapInfo.bmiHeader.biBitCount = 32;
  bitmapInfo.bmiHeader.biCompression = 0;
  bitmapInfo.bmiHeader.biPlanes = 1;
  bitmapInfo.bmiHeader.biSize = 40;
  bitmapInfo.bmiHeader.biWidth = szWnd.cx;
  bitmapInfo.bmiHeader.biHeight = szWnd.cy;

  return CreateDIBSection(dc, &bitmapInfo, DIB_RGB_COLORS, nullptr, 0, 0);
}