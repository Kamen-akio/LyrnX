#pragma once
#include "IInheritable.h"
#include "object.h"

#include "../core/renderManager.h"

namespace Control {

const int _CONTROL_SCREEN_ID_ = 2;

class screen : public object, public IInheritable {
 public:
  screen() : m_swapChain(nullptr){};
  ~screen();

  void Create(HWND hWnd);
  bool EventPrcessor(UINT uMsg, WPARAM wParam, LPARAM lParam);

  protected:
  Core::CRenderManager* m_swapChain;
};
}  // namespace Control