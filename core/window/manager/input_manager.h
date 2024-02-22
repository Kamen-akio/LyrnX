#pragma once
#include "../../dependencies/input.h"
#include "../render.h"

#ifndef _WINDOW_INPUT_MANAGER_H_
#define _WINDOW_INPUT_MANAGER_H_

typedef struct __tagChunk {
  size_t size;
  size_t position;
  
  bool Empty() const { return size == NULL and position == NULL; };
} Chunk;

class InputContainer : public RenderUnit {
 public:
  InputContainer(HWND window, UnitTree* tree);

  void Render(Graphics* graph);

  void GlobalEventHandler(UINT uMsg, WPARAM wp, LPARAM lp);

  int GetCurrentPosition() const;

  std::wstring GetContextOfLine();

 private:
  int currentPos{};
  Chunk selectChunk{};
  std::wstring context{};
};

#endif  // _WINDOW_INPUT_MANAGER_H_