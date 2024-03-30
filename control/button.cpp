#include "button.h"

#include "screen.h"

void _EventCallback();
static Control::button* test{};

Control::button::button() {
  m_prototype[ButtonAlignment] = ButtonAlignmentCenter;
  m_prototype[ButtonLineAlignment] = ButtonLineAlignmentCenter;

  // Register Events

  test = this;

  RegisterEventHandler(Any, (void*)&_EventCallback);
}

void Control::button::SetClickHandler(templClickCallback* ptrFunc) {
  m_eventHandlerMap[EventHandlerID::Click].push_back(ptrFunc);
}

void Control::button::SetPrototype(ButtonPrototypeID id, ButtonPrototype data) {
  m_prototype[id] = data;
}

bool Control::button::_OnPaint(Graphics& srcMap) {
  Graphics grap(m_paintPaint);

  StringFormat stringFormat(StringFormatFlagsNoWrap, 0);
  SolidBrush bgBrush(m_isLeftBtnDown  ? Color::BlueViolet
                     : m_isMouseHover ? Color::DarkCyan
                                      : Color::DarkKhaki);
  SolidBrush ftBrush(Color::White);
  Font ftFont(L"Segoe UI", 16);

  stringFormat.SetAlignment((StringAlignment)m_prototype[ButtonAlignment]);
  stringFormat.SetLineAlignment(
      (StringAlignment)m_prototype[ButtonLineAlignment]);

  grap.FillRectangle(&bgBrush, 0.f, 0.f, m_rcObject.Width, m_rcObject.Height);
  grap.DrawString(m_context.c_str(), m_context.length(), &ftFont,
                  RectF(0.f, 0.f, m_rcObject.Width, m_rcObject.Height),
                  &stringFormat, &ftBrush);

  srcMap.DrawImage(m_paintPaint, m_rcObject);
  return true;
}

void _EventCallback() {
  return;
  /*
  auto b = test->GetRect();
  ((Control::screen*)(test->m_lastContext).screen)
      ->EventPrcessor(WM_PAINT, TRUE, (LPARAM)(void*)&b);
  */
}
