#pragma once
#include <Windows.h>
#include <Windowsx.h>
#include <gdiplus.h>

#include <string>
#include <unordered_map>

#include "./native.h"

using std::unordered_map;
using std::vector;
using std::wstring;

#ifndef _CONTROL_OBJECT_H_
#define _CONTROL_OBJECT_H_

namespace Control {

const int _CONTROL_ELEMENT_ID_ = 0;

class object {
 public:
  object();
  ~object();

  void Create(RectF);
  bool IsMouseInObject(LPARAM);

  void CallAllEventHandlers(EventHandlerID eventID);
  void RegisterEventHandler(EventHandlerID eventID, void* callback);
  void UnregisterEventHandler(EventHandlerID eventID, void* callback);

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

  void SetPrototype(wstring key, wstring data);
  wstring GetPrototype(wstring key) const {
    auto pair = m_prototype.find(key);

    if (pair == m_prototype.end()) {
      return L"";
    }

    return pair->second;
  };
  void ToggleFlags(ObjectFlagID);
  bool HasFlags(ObjectFlagID);

  virtual bool _OnPaint(Graphics&);

  // Event

  bool EventPrcessor(UINT uMsg, WPARAM wParam, LPARAM lParam);

 protected:
  RectF m_rcObject{};
  wstring m_context{};
  unordered_map<wstring, wstring> m_prototype{};

  bool m_bFocus = false;
  bool m_bEnable = true;
  bool m_bVisible = false;

  bool m_isMouseHover = false;
  bool m_isLeftBtnDown = false;

  Bitmap* m_paintPaint = nullptr;
  void* m_renderScreen = nullptr;

  unordered_map<int, vector<void*>> m_eventHandlerMap{};
};
}  // namespace Control

#endif  // !_CONTROL_OBJECT_H_
