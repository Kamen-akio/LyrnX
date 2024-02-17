#pragma once
#include "../../dependencies/input.h"
#include "../render.h"

#ifndef _WINDOW_INPUT_MANAGER_H_
#define _WINDOW_INPUT_MANAGER_H_

class InputContainer : public RenderUnit {
 public:
  InputContainer(HWND window, UnitTree* tree);

  void Render(Graphics* graph);

  void GlobalEventHandler(UINT uMsg, WPARAM wp, LPARAM lp);

 private:
  std::wstring inputData{};
};

#endif  // _WINDOW_INPUT_MANAGER_H_