#pragma once
#include "../utils/node.h"
#include "render.h"

#ifndef _DEPEND_RENDER_TEMPLATE_H_
#define _DEPEND_RENDER_TEMPLATE_H_

#define UNIT_OBJECT_COMPONENT 0x0

#define IsMouseMsg(msg) WM_MOUSEFIRST <= msg&& msg <= WM_MOUSELAST
#define WM_REPAINT WM_USER + WM_PAINT

class RenderHandler abstract {
 public:
  RenderHandler(HWND window, INodeProp* root) {
    _OwnerRoot = root;
    hWindow = window;
  };
  virtual void Render(Graphics*){};

 public:
  virtual void MouseEventHandler(UINT uMsg, WPARAM wParam, LPARAM lParam){};
  virtual void GlobalEventHandler(UINT uMsg, WPARAM wParam, LPARAM lParam){};

  void _handler(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_CREATE) {
      rcUnit = Rect(*(Rect*)(void*)lParam);
      pUnitBitmap = new Bitmap(rcUnit.Width, rcUnit.Height);
      return;
    }

    if (uMsg == WM_DESTROY) {
      delete pUnitBitmap;
      pUnitBitmap = nullptr;
    }

    if (uMsg == WM_REPAINT) {
      ((Graphics*)lParam)->DrawImage(pUnitBitmap, rcUnit);
    }

    if (uMsg == WM_PAINT) {
      Graphics graphics(pUnitBitmap);
      graphics.Clear(Color::Transparent);
      graphics.SetSmoothingMode(SmoothingModeHighQuality);
      Render(&graphics);
    }

    // Event

    if (uMsg == WM_LBUTTONDOWN or uMsg == WM_LBUTTONUP) {
      isLeftBtnDown = uMsg == WM_LBUTTONDOWN;
    }

    if (uMsg == WM_SETFOCUS || uMsg == WM_KILLFOCUS) {
      isFocus = (uMsg == WM_SETFOCUS);
    }

    if (IsMouseMsg(uMsg)) {
      MouseEventHandler(uMsg, wParam, lParam);
    }

    GlobalEventHandler(uMsg, wParam, lParam);
  }

 private:
  RenderHandler(RenderHandler&) = delete;
  RenderHandler& operator=(RenderHandler&) = delete;

 public:
  HWND hWindow{};
  Rect rcUnit{};
  bool isFocus = false;
  bool isLeftBtnDown = false;

 private:
  INodeProp* _OwnerRoot{};

  Bitmap* pUnitBitmap{};
};

class RenderUnit : public RenderHandler {
 public:
  RenderUnit(HWND window, UnitTree* tree)
      : RenderHandler(window, tree->GetOwner()) {
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

 public:
  int UnitType = UNIT_OBJECT_COMPONENT;

 private:
  UnitTree* _RenderTree{};
};

#endif  // !_DEPEND_RENDER_TEMPLATE_H_
