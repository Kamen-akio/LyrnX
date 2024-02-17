#pragma once
#include "../render.h"

// Default: 32 (px)

#ifndef WINDOW_BACKGROUND_ROUNDED_RADIUS
#define WINDOW_BACKGROUND_ROUNDED_RADIUS 32
#endif  // WINDOW_BACKGROUND_ROUNDED_RADIUS

#ifndef _WINDOW_RENDER_MANAGER_H_
#define _WINDOW_RENDER_MANAGER_H_

class RenderManager : public RenderUnit {
 public:
  RenderManager(HWND window, UnitTree* tree);

  void Render(Graphics* graph);

 public:
  bool useRounded = Environment::GetEnv().EnableRoundedFrame;
};

#endif  // !_WINDOW_RENDER_MANAGER_H_