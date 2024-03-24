#pragma once
#include "object.h"

#ifndef _CONTROL_BUTTON_H_
#define _CONTROL_BUTTON_H_

namespace Control {

const int _CONTROL_BUTTON_ID_ = 1;

class button : public object {
 public:
  button(){};
  void SetClickHandler(templClickCallback*);
  bool _OnPaint(Graphics&) override;
};
}  // namespace Control

#endif  // !_CONTROL_BUTTON_H_