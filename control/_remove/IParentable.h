#pragma once
#include <vector>

#include "element.h"

using std::vector;

#ifndef _CONTROL_INTERFACE_PARENTABLE_H_
#define _CONTROL_INTERFACE_PARENTABLE_H_

namespace Control {
class IParentable {
 public:
  vector<Element> m_childElements;
};
}  // namespace Control

#endif  // !_CONTROL_INTERFACE_PARENTABLE_H_
