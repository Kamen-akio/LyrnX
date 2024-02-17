#include "render_manager.h"

RenderManager::RenderManager(HWND window, UnitTree* tree) : RenderUnit(window, tree){};

void RenderManager::Render(Graphics* graph) {
  SolidBrush bgBrush(Color::BurlyWood);
  if (useRounded) {
    PathExt bgPath{};

    bgPath.AddBezierRounded(WINDOW_BACKGROUND_ROUNDED_RADIUS, rcUnit);
    graph->FillPath(&bgBrush, &bgPath);
  } else
    graph->FillRectangle(&bgBrush, rcUnit);
}