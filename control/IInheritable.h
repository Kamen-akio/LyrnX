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

    if (uMsg != WM_PAINT) {
      for (auto& obj : m_childObject) {
        if (WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST ||
            uMsg == WM_MOUSELEAVE) {
          if (obj->IsMouseInObject(lParam)) {
            if (not obj->HasFlags(Control::MouseHover)) {
              obj->ToggleFlags(Control::MouseHover);
            }
            result |= obj->EventPrcessor(uMsg, wParam, lParam);

            continue;
          }
          
          if (obj->HasFlags(Control::MouseHover)) {
            obj->ToggleFlags(Control::MouseHover);
            result |= obj->EventPrcessor(uMsg, wParam, lParam);
          }

        }
      }

      return result;
    }

    RenderContext* renderContext = (RenderContext*)(void*)lParam;
    RectF targetArea = renderContext->targetRect;
    Graphics* srcGrap = renderContext->graphics;

    for (auto& obj : m_childObject) {
      /*
      if (!targetArea.IsEmptyArea() &&
          not obj->GetRect().IntersectsWith(targetArea)) {
        continue;
      }
      */

      auto status =
          srcGrap->BeginContainer(obj->GetRect(), obj->GetRect(), UnitPixel);

      result |= obj->EventPrcessor(uMsg, wParam, lParam);

      srcGrap->EndContainer(status);
    }

    return result;
  };

  void SetChild(object* obj) { m_childObject.push_back(obj); };
  vector<object*> m_childObject{};

 private:
  object* m_pLastFocusControl{};
};
}  // namespace Control

#endif  // !_CONTROL_INTERFACE_IINHERITABLE_H_