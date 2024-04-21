#pragma once
#include "Object.h"

using namespace Gdiplus;

#ifndef _CONTROL_BASE_
#define _CONTROL_BASE_

#define IS_MOUSE_MSG(msg)                           \
  ((WM_MOUSEFIRST <= msg && msg <= WM_MOUSELAST) || \
   (WM_NCMOUSEHOVER <= msg && msg <= WM_MOUSELEAVE))

namespace Component {

typedef struct __tagRenderEnvironmentBlock {
  Graphics* presentGraphics;
} REB, RenderEnvironmentBlock;

class ControlBase : public Object {
 public:
  ControlBase();
  ControlBase(RectF rectObject);
  ~ControlBase();
  void Destroy();

 public:

  int GetStatusRaw();
  void SetStatus(int);
  void DelStatus(int);
  bool HasStatus(int) const;

  RectF GetControlRect() const;
  void SetControlRect(RectF);
  SizeF GetControlSize();
  void SetControlSize(SizeF);
  PointF GetControlPosition();
  void SetControlPosition(PointF);

  bool IsProcessed();
  LRESULT MessageProcess(UINT uMsg, WPARAM wParam, LPARAM lParam);
  ControlBase* HitTest(LPARAM lParam);
  ControlBase* HitTest(PointF ptMouse);

  virtual bool OnPaint(Graphics*);
  virtual LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam,
                               LPARAM lParam);  // 未处理鼠标消息走此函数

  virtual void SetChild(ControlBase*){};  // 虚函数，部分组件应不支持子组件
  virtual LRESULT CallChildrenProc(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return NULL;
  };

 protected:
  int m_status;
  bool m_messageProcessed;

  RectF m_rcObject;
  vector<ControlBase*> m_childenList;
};
}  // namespace Component

#endif  // !_CONTROL_BASE_
