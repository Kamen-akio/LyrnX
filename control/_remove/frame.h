#pragma once
#include "IParentable.h"
#include "element.h"

#ifndef _CONTROL_FRAME_H_
#define _CONTROL_FRAME_H_

namespace Control {
class Frame : public Element, public IParentable {
 public:
  Frame(REAL x, REAL y, REAL width, REAL height)
      : Element(x, y, width, height) {}

  Frame(PointF posElement, SizeF szElement)
      : Element(posElement, szElement) {}

  Frame(RectF rcElement) : Element(rcElement) {}

  Frame() : Element(){};

  // Process

  virtual Bitmap _OnPaint() override;

  // Event

  virtual void _OnPosChanged(PointF newPosition) override;
  virtual void _OnSizeChanged(SizeF newSize) override;
};
}  // namespace Control

#endif  // !_CONTROL_FRAME_H_
