#pragma once
#include "./Control.h"

#ifndef _COMPONENT_BUTTON_H_
namespace Component {

typedef enum {
  ButtonStatus_ButtonDown = 0b01,
  ButtonStatus_ButtonOver = 0b10
} __ButtonStatus;

class Button : public ControlBase {
 public:
  Button();
  ~Button();

 public:
  void SetContext(wstring context);

  bool OnPaint(Graphics*);
  LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

 protected:
  wstring m_context;
};
}  // namespace Component
#endif  // !_COMPONENT_BUTTON_H_
