#include "object.h"

#include <windowsx.h>

Control::object::~object() {
  if (m_paintPaint != nullptr) {
    delete m_paintPaint;
    m_paintPaint = nullptr;
  }
}

void Control::object::Create(RectF rcObj) {
  m_rcObject = RectF(rcObj);
  m_paintPaint = new Bitmap(m_rcObject.Width, m_rcObject.Height);
}

bool Control::object::IsMouseInObject(LPARAM lParam) {
  if (m_rcObject.IsEmptyArea()) return false;

  return m_rcObject.Contains(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
}

void Control::object::SetPos(PointF ptObj) {
  m_rcObject.X = ptObj.X;
  m_rcObject.Y = ptObj.Y;
}

void Control::object::SetSize(SizeF szObj) {
  if (m_paintPaint != nullptr) {
    delete m_paintPaint;
    m_paintPaint = nullptr;
  }

  m_rcObject.Width = szObj.Width;
  m_rcObject.Height = szObj.Height;
  m_paintPaint = new Bitmap(m_rcObject.Width, m_rcObject.Height);
}

void Control::object::SetRect(RectF rcObj) {
  if (m_paintPaint != nullptr) {
    delete m_paintPaint;
    m_paintPaint = nullptr;
  }

  m_rcObject = rcObj;
  m_paintPaint = new Bitmap(m_rcObject.Width, m_rcObject.Height);
}

bool Control::object::_OnPaint(Graphics &) { return false; }

bool Control::object::EventPrcessor(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (not m_bEnable or not IsVisible()) return false;

  if (uMsg == WM_MOUSEMOVE) {
    return m_isHover = IsMouseInObject(lParam);
  }

  if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP) {
    if (uMsg == WM_LBUTTONDOWN) {
      return m_isLeftBtnDown = IsMouseInObject(lParam);
    }

    if (not m_isLeftBtnDown) return false;

    m_isLeftBtnDown = false;

    auto handlerPair = m_eventHandlerMap.find(EventHandlerID::Click);
    if (handlerPair == m_eventHandlerMap.end()) return true;

    auto callback = (templClickCallback *)handlerPair->second;
    if (callback == nullptr) return true;

    callback();
  }

  if (uMsg == WM_PAINT) {
    return _OnPaint(*(Graphics *)(void *)lParam);
  }
}