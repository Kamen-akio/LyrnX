#pragma once
#include <Windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

#ifndef _CONTROL_ELEMENT_H_
#define _CONTROL_ELEMENT_H_

namespace Control {
class Element {
 public:
  Element(REAL x, REAL y, REAL width, REAL height)
      : m_rcElement(x, y, width, height),
        m_bVisible(false),
        m_bDisable(false),
        m_bFocus(false) {
    m_layoutBitmap = new Bitmap(m_rcElement.Width, m_rcElement.Height);
  };
  Element(PointF posElement, SizeF szElement)
      : m_rcElement(posElement, szElement),
        m_bVisible(false),
        m_bDisable(false),
        m_bFocus(false) {
    m_layoutBitmap = new Bitmap(m_rcElement.Width, m_rcElement.Height);
  };
  Element(RectF rcElement)
      : m_rcElement(rcElement),
        m_bVisible(false),
        m_bDisable(false),
        m_bFocus(false) {
    m_layoutBitmap = new Bitmap(m_rcElement.Width, m_rcElement.Height);
  };
  Element()
      : m_rcElement({}), m_bVisible(false), m_bDisable(false), m_bFocus(false) {
    m_layoutBitmap = new Bitmap(m_rcElement.Width, m_rcElement.Height);
  };

  virtual void Create() = 0;

  // Process

  virtual Bitmap _OnPaint() = 0;

  // Event

  virtual void _OnPosChanged(PointF newPosition) = 0;
  virtual void _OnSizeChanged(SizeF newSize) = 0;

  // Get
  RectF GetElementRect() const { return m_rcElement; }

 protected:
  RectF m_rcElement;
  bool m_bVisible;
  bool m_bDisable;
  bool m_bFocus;

  Bitmap* m_layoutBitmap;
};

struct EventID {
  enum Button { LeftButton, MiddleButton, RightButon, XButton };
};

}  // namespace Control

#endif  // !_CONTROL_ELEMENT_H_
