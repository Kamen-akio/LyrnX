#include "./frame.h"

Bitmap Control::Frame::_OnPaint() {
  if (m_childElements.empty()) return ;

  Graphics grap(m_layoutBitmap);

  for (auto &element : m_childElements) {
    Graphics target(&element._OnPaint());
    BitBlt(grap.GetHDC(),)
    (element._OnPaint(), element.GetElementRect());
  }

  return ;
}

void Control::Frame::_OnPosChanged(PointF newPosition) {}

void Control::Frame::_OnSizeChanged(SizeF newSize) {}
