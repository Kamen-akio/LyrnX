#pragma once
#include "../dependencies/render.h"

namespace Gdiplus {

class DoubleBuffer {
 private:
  HDC _hVirtualDC{};
  HBITMAP _hVirtualBitmap{};
  HBITMAP _hLastVirtualBitmap{};

  HWND _hWnd{};
  SIZE _szWnd{};
  POINT _posWnd{};

 public:
  DoubleBuffer(HWND hWnd);
  ~DoubleBuffer();

  HDC& GetRenderDC();

  void Flush();

  void FlushPos(POINT posWnd);

  void FlushSize(SIZE szWnd);

  void FlushByApi();

 private:
  DoubleBuffer(DoubleBuffer&) = delete;
  DoubleBuffer& operator=(DoubleBuffer&) = delete;
};

}  // namespace Gdiplus