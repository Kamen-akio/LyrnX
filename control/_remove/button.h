#pragma once
#include "element.h"

#ifndef _CONTROL_BUTTON_H_
#define _CONTROL_BUTTON_H_

namespace Control {
class Button : public Element {
 public:
  // Process

  virtual Bitmap _OnPaint() override;

  // Event

  virtual void _OnClick(int) = 0;
};
}  // namespace Control

#endif  // !_CONTROL_BUTTON_H_
