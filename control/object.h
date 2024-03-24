#pragma once
#include <Windows.h>
#include <gdiplus.h>

#include <string>
#include <unordered_map>

using std::unordered_map;
using std::vector;
using std::wstring;
using namespace Gdiplus;

#ifndef _CONTROL_OBJECT_H_
#define _CONTROL_OBJECT_H_

namespace Control {

const int _CONTROL_ELEMENT_ID_ = 0;

typedef enum { Click, User } EventHandlerID;

typedef void __stdcall templClickCallback();

class object {
 public:
  object(){};
  ~object();

  void Create(RectF);
  bool IsMouseInObject(LPARAM);

  void SetContext(wstring data) { m_context = data; };
  wstring GetContext() const { return m_context; }
  bool IsEnable() const { return m_bEnable; };
  void SetEnable(bool status) { m_bEnable = status; };
  bool IsVisible() const { return m_bVisible; };
  void SetVisible(bool status) { m_bVisible = status; };

  void SetPos(PointF);
  void SetSize(SizeF);
  void SetRect(RectF);
  RectF GetRect() const { return m_rcObject; };

  virtual bool _OnPaint(Graphics&);

  // Event

  bool EventPrcessor(UINT uMsg, WPARAM wParam, LPARAM lParam);

 protected:
  int m_objectID = 0;
  RectF m_rcObject = {};
  wstring m_context{};

  bool m_bEnable = true;
  bool m_bVisible = false;
  bool m_isHover = false;
  bool m_isLeftBtnDown = false;

  Bitmap* m_paintPaint = nullptr;

  unordered_map<int, void*> m_eventHandlerMap{};
};
}  // namespace Control

#endif  // !_CONTROL_OBJECT_H_
