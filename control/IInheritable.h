#pragma once
#include <vector>

#include "./object.h"

using namespace std;

#ifndef _CONTROL_INTERFACE_IINHERITABLE_H_
#define _CONTROL_INTERFACE_IINHERITABLE_H_

namespace Control {
class IInheritable {
 public:
  ~IInheritable() {
    if (m_childObject.empty()) return;
    for (auto obj : m_childObject) {
      delete obj;
    }
  }

  bool CallObjectProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    bool result = false;
    for (auto& obj : m_childObject) {
      result = result || obj->EventPrcessor(uMsg, wParam, lParam);
    }

    return result;
  };

  void SetChild(object* obj) { m_childObject.push_back(obj); };
  vector<object*> m_childObject{};
};
}  // namespace Control

#endif  // !_CONTROL_INTERFACE_IINHERITABLE_H_