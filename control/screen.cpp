#include "Screen.h"

#include "..\core\impl\dwmHelper.h"
using namespace Component::PrototypeID;

Component::Screen::Screen() : m_presentWnd(NULL) {}

void Component::Screen::Bind(HWND hWnd) {
  m_presentWnd = hWnd;
  m_protoMap[PrototypeID::PrototypeIdGeneral_BackgroundColor] =
      new int(0x10FFAA01);
}

Component::Screen::~Screen() {}

void Component::Screen::SetChild(ControlBase* child) {
  m_childenList.push_back(child);
}

LRESULT Component::Screen::CallChildrenProc(UINT uMsg, WPARAM wParam,
                                            LPARAM lParam) {
  LRESULT result{};

  if (uMsg == WM_PAINT) {
    for (auto& child : m_childenList) {
      auto grap = reinterpret_cast<REB*>(lParam)->presentGraphics;
      auto childRect = ((ControlBase*)child)->GetControlRect();
      auto stack = grap->BeginContainer();
      grap->TranslateTransform(childRect.X, childRect.Y);
      // grap->SetClip(childRect, CombineModeReplace);

      result |= child->MessageProcess(uMsg, wParam, lParam);

      // grap->ResetClip();
      grap->ResetTransform();
      grap->EndContainer(stack);
    }

    return result;
  }

  for (auto& child : m_childenList) {
    result |= child->MessageProcess(uMsg, wParam, lParam);
  }
  return result;
}

static LRESULT WindowHitTest(HWND hWnd, RectF rect, LPARAM lParam) {
  POINT ptMouse{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
  ScreenToClient(hWnd, &ptMouse);

  if (1 < ptMouse.y && ptMouse.y <= 30) {
    auto const controlRight = rect.Width - 1;
    if (controlRight - 45 <= ptMouse.x && ptMouse.x < controlRight)
      return HTCLOSE;
    if (controlRight - 45 * 2 <= ptMouse.x && ptMouse.x < controlRight - 45)
      return HTMAXBUTTON;
    if (controlRight - 45 * 3 <= ptMouse.x && ptMouse.x < controlRight - 45 * 2)
      return HTMINBUTTON;
  }

  if (ptMouse.y <= 8) {
    if (ptMouse.x <= 8) return HTTOPLEFT;
    if (ptMouse.x >= rect.Width - 8) return HTTOPRIGHT;
    return HTTOP;
  }

  if (ptMouse.y <= 29) {
    if (8 < ptMouse.x && ptMouse.x < rect.Width - 8) return HTCAPTION;
  }

  if (ptMouse.y <= rect.Height - 8) {
    if (ptMouse.x <= 8) return HTLEFT;
    if (ptMouse.x >= rect.Width - 8) return HTRIGHT;
    return HTCLIENT;
  }

  if (ptMouse.x <= 8) return HTBOTTOMLEFT;
  if (ptMouse.x >= rect.Width - 8) return HTBOTTOMRIGHT;
  return HTBOTTOM;
}

LRESULT Component::Screen::MessageProcess(UINT uMsg, WPARAM wParam,
                                          LPARAM lParam) {
  if (not m_presentWnd) return NULL;

  LRESULT dwmResult{};

  if (DwmDefWindowProc(m_presentWnd, uMsg, wParam, lParam, &dwmResult)) {
    m_messageProcessed = true;
    return dwmResult;
  }

  LRESULT status{};
  static ControlBase* lastOverControl{nullptr};
  static ControlBase* lastCaptureControl{nullptr};

  switch (uMsg) {
    case WM_CREATE: {
      RECT rcClient;
      GetWindowRect(m_presentWnd, &rcClient);
      SetWindowPos(m_presentWnd, NULL, rcClient.left, rcClient.top,
                   rcClient.right - rcClient.left,
                   rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);
      break;
    }

    case WM_ACTIVATE: {
      Core::Dwmapi::ExtendFrameIntoClientArea(
          m_renderManager->GetWindowHandle());

      break;
    }

      // No Process.

    case WM_NCCALCSIZE: {
      if (wParam == TRUE) {
        m_messageProcessed = true;
        RECT& r = reinterpret_cast<LPNCCALCSIZE_PARAMS>(lParam)->rgrc[0];
        r.top -= 1;
        r.left -= 1;
        r.right -= 1;
        r.bottom -= 1;
        return 0;
      }

      break;
    }

    case WM_NCHITTEST: {
      m_messageProcessed = true;

      return WindowHitTest(m_presentWnd, m_rcObject, lParam);
    }

    case WM_SIZE: {
      if (wParam == SIZE_RESTORED || wParam == SIZE_MINIMIZED) {
        m_renderManager->ApplyOffset();
      }

      if (wParam == SIZE_MAXIMIZED) {
        m_renderManager->ApplyOffset({8, 8});
      }

      m_rcObject.Width = (float)LOWORD(lParam);
      m_rcObject.Height = (float)HIWORD(lParam);
      m_renderManager->ResizeBuffer(
          {(int)m_rcObject.Width, (int)m_rcObject.Height});

      [[fallthrough]];
    }

    case WM_PAINT: {
      auto targetDC = this->m_renderManager->GetDC();
      Graphics presentGrap(targetDC);
      REB reb{&presentGrap};

      status =
          ((ControlBase*)this)->MessageProcess(WM_PAINT, NULL, (LPARAM)&reb);

      presentGrap.~Graphics();
      m_renderManager->InvalidateRect(m_rcObject);

      if (status || true) {
        m_renderManager->PresentAll();
      }
      return 0;
    }

    case WM_MOUSEMOVE: {
      auto currOverControl = HitTest(lParam);
      if (currOverControl != lastOverControl) {
        if (lastOverControl != nullptr) {
          lastOverControl->MessageProcess(WM_MOUSELEAVE, wParam, lParam);
        }
        currOverControl->MessageProcess(WM_MOUSEHOVER, wParam, lParam);
        lastOverControl = currOverControl;
      }
    }
  }

  if (IS_MOUSE_MSG(uMsg)) {
    PointF ptMouse{GET_X_LPARAM(lParam) * 1.f, GET_Y_LPARAM(lParam) * 1.f};

    if (uMsg == WM_LBUTTONDOWN) {
      SetCapture(m_presentWnd);
      if (lastCaptureControl == nullptr) lastCaptureControl = HitTest(ptMouse);
    }

    auto target = lastCaptureControl;
    if (target == nullptr) {
      target = HitTest(ptMouse);
    }

    if (uMsg == WM_LBUTTONUP) {
      ReleaseCapture();
      lastCaptureControl = nullptr;
    }

    return target->OnMouseEvent(uMsg, wParam, lParam);
  }

  return ((ControlBase*)this)->MessageProcess(uMsg, wParam, lParam);
}

bool Component::Screen::OnPaint(Graphics* presentGraphics) {
  presentGraphics->Clear(
      *(int*)GetPrototype(PrototypeIdGeneral_BackgroundColor));
  return true;  // Always return false only
}
