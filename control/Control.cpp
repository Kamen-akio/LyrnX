#include "Control.h"

Component::ControlBase::ControlBase() {
  m_status = NULL;
  m_rcObject = RectF();
  m_childenList = vector<ControlBase *>();
}

Component::ControlBase::ControlBase(RectF rectObject) {
  m_status = NULL;
  m_rcObject = RectF(rectObject);
  m_childenList = vector<ControlBase *>();
}

void Component::ControlBase::Destroy() {
  for (auto &child : m_childenList) {
    child->Destroy();
  }
}

int Component::ControlBase::GetStatusRaw() { return m_status; }

void Component::ControlBase::SetStatus(int status) { m_status |= status; }

void Component::ControlBase::DelStatus(int status) { m_status &= ~status; }

bool Component::ControlBase::HasStatus(int status) const {
  return (m_status & status) == status;
}

RectF Component::ControlBase::GetControlRect() const { return m_rcObject; }

void Component::ControlBase::SetControlRect(RectF rect) {
  // todo, repaint.
  m_rcObject = rect;
}

SizeF Component::ControlBase::GetControlSize() {
  return {m_rcObject.Width, m_rcObject.Height};
}

void Component::ControlBase::SetControlSize(SizeF size) {
  m_rcObject.Width = size.Width;
  m_rcObject.Height = size.Height;
}

PointF Component::ControlBase::GetControlPosition() {
  return {m_rcObject.X, m_rcObject.Y};
}

void Component::ControlBase::SetControlPosition(PointF position) {
  m_rcObject.X = position.X;
  m_rcObject.Y = position.Y;
}

Component::ControlBase::~ControlBase() {}

bool Component::ControlBase::IsProcessed() { 
  if (m_messageProcessed) {
    m_messageProcessed = false;
    return true;
  }

  return false;
}

LRESULT Component::ControlBase::MessageProcess(UINT uMsg, WPARAM wParam,
                                               LPARAM lParam) {
  LRESULT status{};
  if (uMsg == WM_PAINT) {
    auto reb = reinterpret_cast<RenderEnvironmentBlock *>(lParam);
    status |= (LRESULT)OnPaint(reb->presentGraphics);
    status |= CallChildrenProc(uMsg, wParam, lParam);

    return status;
  }

  if (IS_MOUSE_MSG(uMsg)) {
    PointF ptMouse{GET_X_LPARAM(lParam) * 1.f, GET_Y_LPARAM(lParam) * 1.f};

    auto target = HitTest(ptMouse);

    return target->OnMouseEvent(uMsg, wParam, lParam);
  }

  return FALSE;
}

Component::ControlBase *Component::ControlBase::HitTest(LPARAM lParam) {
  return HitTest({GET_X_LPARAM(lParam) * 1.f, GET_Y_LPARAM(lParam) * 1.f});
}

Component::ControlBase *Component::ControlBase::HitTest(PointF ptMouse) {
  ControlBase *retControl = nullptr;

  for (auto &child : m_childenList) {
    if (child->GetControlRect().Contains(ptMouse) && retControl == nullptr) {
      retControl = child->HitTest(ptMouse);
      break;
    }
  }

  if (retControl == nullptr) {
    retControl = this;
  }

  return retControl;
}

bool Component::ControlBase::OnPaint(Graphics *) { return false; }

LRESULT Component::ControlBase::OnMouseEvent(UINT uMsg, WPARAM wParam,
                                             LPARAM lParam) {
  return NULL;
}
