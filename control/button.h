#pragma once
#include "object.h"

#ifndef _CONTROL_BUTTON_H_
#define _CONTROL_BUTTON_H_

namespace Control {

const int _CONTROL_BUTTON_ID_ = 1;

typedef enum {
  ButtonAlignment = 0,
  ButtonLineAlignment = 1,
} ButtonPrototypeID;

typedef enum {
  ButtonInvalid = -1,

  ButtonAlignmentNear = StringAlignmentNear,
  ButtonAlignmentCenter,
  ButtonAlignmentFar,

  ButtonLineAlignmentNear = StringAlignmentNear,
  ButtonLineAlignmentCenter,
  ButtonLineAlignmentFar,
} ButtonPrototype;

class button : public object {
 public:
  button();
  void SetClickHandler(templClickCallback*);
  void SetPrototype(ButtonPrototypeID, ButtonPrototype);
  ButtonPrototype GetPrototype(ButtonPrototypeID id) {
    auto pair = m_prototype.find(id);
    if (pair == m_prototype.end()) {
      return ButtonInvalid;
    }

    return pair->second;
  };

  bool _OnPaint(Graphics&) override;
 public:

  protected:
  unordered_map<ButtonPrototypeID, ButtonPrototype> m_prototype{};
};
}  // namespace Control

#endif  // !_CONTROL_BUTTON_H_