#include "./Button.h"
using namespace Component::PrototypeID;

Component::Button::Button() {
  m_context = L"°´Å¥";

  SetPrototype(PrototypeIdGeneral_FontColor, new int(Color::Black));
  SetPrototype(PrototypeIdGeneral_BackgroundColor, new int(Color::Blue));
  SetPrototype(PrototypeIdGeneral_BackgroundHoverColor,
               new int(Color::LightBlue));
  SetPrototype(PrototypeIdGeneral_BackgroundActiveColor, new int(Color::Cyan));
}

Component::Button::~Button() {}

bool Component::Button::OnPaint(Graphics *grap) {
  grap->SetSmoothingMode(SmoothingModeHighQuality);

  Font ftFront(L"Segoe UI", 16);

  SolidBrush ftBrush(*(int *)GetPrototype(PrototypeIdGeneral_FontColor));
  SolidBrush bgBrush(
      HasStatus(ButtonStatus_ButtonDown)
          ? *(int *)GetPrototype(PrototypeIdGeneral_BackgroundActiveColor)
      : HasStatus(ButtonStatus_ButtonOver)
          ? *(int *)GetPrototype(PrototypeIdGeneral_BackgroundHoverColor)
          : *(int *)GetPrototype(PrototypeIdGeneral_BackgroundColor));

  StringFormat strFormat{};
  strFormat.SetAlignment(StringAlignmentCenter);
  strFormat.SetLineAlignment(StringAlignmentCenter);

  // grap->FillRectangle(&bgBrush, 0.f, 0.f, GetControlSize().Width,
  //                     GetControlSize().Height);

  GraphicsPath path{};
  CreateRoundedPath(
      path, RectF(0.f, 0.f, GetControlSize().Width, GetControlSize().Height),
      16);
  grap->FillPath(&bgBrush, &path);

  grap->DrawString(
      m_context.c_str(), (int)m_context.length(), &ftFront,
      RectF(0.f, 0.f, GetControlSize().Width, GetControlSize().Height),
      &strFormat, &ftBrush);

  return true;
}

LRESULT Component::Button::OnMouseEvent(UINT uMsg, WPARAM wParam,
                                        LPARAM lParam) {
  PointF ptMouse{GET_X_LPARAM(lParam) * 1.f, GET_Y_LPARAM(lParam) * 1.f};

  if (uMsg == WM_MOUSELEAVE) {
    DelStatus(ButtonStatus_ButtonOver);
    m_renderManager->InvalidateRect(m_rcObject);
  }

  /* ~~~
  if (not m_rcObject.Contains(ptMouse)) {
    return NULL;
  }
  */

  if (uMsg == WM_LBUTTONDOWN) {
    SetStatus(ButtonStatus_ButtonDown);
    m_renderManager->InvalidateRect(m_rcObject);
  }

  if (uMsg == WM_LBUTTONUP) {
    DelStatus(ButtonStatus_ButtonDown);
    m_renderManager->InvalidateRect(m_rcObject);
  }

  if (uMsg == WM_MOUSEHOVER) {
    SetStatus(ButtonStatus_ButtonOver);
    m_renderManager->InvalidateRect(m_rcObject);
  }

  return FALSE;
}
