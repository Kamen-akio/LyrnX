#pragma once
#include "../utils/node.h"
#include "render.h"

#ifndef _DEPEND_RENDER_TEMPLATE_H_
#define _DEPEND_RENDER_TEMPLATE_H_

#define WM_CREATE_UNIT WM_USER + WM_CREATE
#define WM_DESTROY_UNIT WM_USER + WM_DESTROY
#define WM_SIZE_UNIT WM_USER + WM_SIZE
#define WM_MOVE_UNIT WM_USER + WM_MOVE
#define WM_REPAINT_UNIT WM_USER + WM_PAINT

class RenderHandler abstract {
 public:
  RenderHandler(INodeProp* root) { _OwnerRoot = root; };
  virtual void Render(Graphics*){};

 public:
  void _handler(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_CREATE_UNIT) {
      rcUnit = Rect(*(Rect*)(void*)lParam);
      pUnitBitmap =
          new Bitmap(rcUnit.Width, rcUnit.Height, PixelFormat32bppARGB);
      return;
    }

    if (uMsg == WM_DESTROY_UNIT) {
      delete pUnitBitmap;
      pUnitBitmap = nullptr;
    }

    if (uMsg == WM_PAINT) {
      auto graphics = (Graphics*)lParam;
      graphics->DrawImage(pUnitBitmap, rcUnit);
    }

    if (uMsg == WM_REPAINT_UNIT) {
      auto graphics = Graphics(pUnitBitmap);
      graphics.SetSmoothingMode(SmoothingModeHighQuality);
      Render(&graphics);
      graphics.Save();
      return;
    }

    // Event

    if (uMsg == WM_LBUTTONDOWN) {
      isLeftBtnDown = true;
    }

    if (uMsg == WM_LBUTTONUP) {
      isLeftBtnDown = false;
    }
  }

 private:
  RenderHandler(RenderHandler&) = delete;
  RenderHandler& operator=(RenderHandler&) = delete;

 public:
  Rect rcUnit{};
  bool isLeftBtnDown = false;

 private:
  INodeProp* _OwnerRoot{};

  Bitmap* pUnitBitmap{};
};

class RenderUnit : public RenderHandler {
 public:
  RenderUnit(UnitTree* tree) : RenderHandler(tree->GetOwner()) {
    _RenderTree = tree;
  };

  void SetParent(RenderUnit* parent, INodeProp* prop) {
    auto parentNode = _RenderTree->Find(parent), selfNode = prop;

    if (parentNode->childUnit) {
      auto brotherNode = parentNode->childUnit;

      while (brotherNode != nullptr) {
        if (brotherNode->broUnit != nullptr) {
          brotherNode = brotherNode->broUnit;
          continue;
        }

        brotherNode->broUnit = selfNode;
        break;
      }
      return;
    }

    parentNode->childUnit = selfNode;
  }

 private:
  RenderUnit(RenderUnit&) = delete;
  RenderUnit& operator=(RenderUnit&) = delete;

 private:
  UnitTree* _RenderTree{};
};

#endif  // !_DEPEND_RENDER_TEMPLATE_H_
