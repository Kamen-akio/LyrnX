#include "button.h"

void Control::button::SetClickHandler(templClickCallback * ptrFunc) {
  m_eventHandlerMap[EventHandlerID::Click] = ptrFunc;
}

bool Control::button::_OnPaint(Graphics& srcMap) {
  Graphics grap(m_paintPaint);

  StringFormat centerFormat(StringFormatFlagsNoWrap, 0);
  SolidBrush bgBrush(m_isLeftBtnDown ? Color::BlueViolet : m_isHover ? Color::DarkCyan : Color::DarkKhaki);
  SolidBrush ftBrush(Color::White);
  Font ftFont(L"Segoe UI", 16);

  centerFormat.SetAlignment(StringAlignmentCenter);
  centerFormat.SetLineAlignment(StringAlignmentCenter);

  grap.FillRectangle(&bgBrush, 0.f, 0.f, m_rcObject.Width, m_rcObject.Height);
  grap.DrawString(m_context.c_str(), m_context.length(), &ftFont,
                  RectF(0.f, 0.f, m_rcObject.Width, m_rcObject.Height), &centerFormat,
                  &ftBrush);

  srcMap.DrawImage(m_paintPaint, m_rcObject);
  return true;
}