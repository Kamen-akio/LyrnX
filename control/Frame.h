#pragma once
#include "Control.h"

#ifndef _OBJECT_FRAME_H_
#define _OBJECT_FRAME_H_
namespace Component {
class Frame : public Component::ControlBase {
 public:
  Frame();
  ~Frame();
  void Destroy();
};
}  // namespace Component

#endif  // !_OBJECT_FRAME_H_
