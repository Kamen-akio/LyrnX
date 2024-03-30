#include "object.h"

#include <windowsx.h>

static unordered_map<Control::object *, int> allObjectFlags{};

Control::object::object() { allObjectFlags[this] = 0; }

Control::object::~object() {
  if (m_paintPaint != nullptr) {
    delete m_paintPaint;
    m_paintPaint = nullptr;
  }

  allObjectFlags.erase(this);
}

void Control::object::Create(RectF rcObj) {
  m_rcObject = RectF(rcObj);
  m_paintPaint = new Bitmap(m_rcObject.Width, m_rcObject.Height);
}

bool Control::object::IsMouseInObject(LPARAM lParam) {
  if (m_rcObject.IsEmptyArea() && not m_bVisible) return false;

  return m_rcObject.Contains(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
}

/*
 * Todo. CallAllEventHandlers Method needs to support multi-arguments to push
 * any arguments.
 */

void Control::object::CallAllEventHandlers(EventHandlerID eventID) {
  auto &callbacks = m_eventHandlerMap[eventID];

  if (&callbacks == nullptr || callbacks.empty()) {
    return;
  }

  for (auto callback : callbacks) {
    if (callback == nullptr) {
      continue;
    }

    try {
      ((templClickCallback *)callback)();
    } catch (const std::exception &err) {
      throw err;
    }
  }
}

void Control::object::RegisterEventHandler(EventHandlerID eventID,
                                           void *callback) {
  auto &callbacks = m_eventHandlerMap[eventID];
  if (&callbacks == nullptr) {
    m_eventHandlerMap[eventID] = vector<void *>();
  }

  m_eventHandlerMap[eventID].push_back(callback);
}

void Control::object::UnregisterEventHandler(EventHandlerID eventID,
                                             void *callback) {
  auto callbacks = &m_eventHandlerMap[eventID];
  if (callbacks == nullptr) {
    m_eventHandlerMap[eventID] = vector<void *>();
    return;
  }

  for (auto item = callbacks->begin(); item != callbacks->end(); item++) {
    if (*item._Ptr == callback) {
      callbacks->erase(item);
      break;
    }
  }
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

void Control::object::SetPrototype(wstring key, wstring data) {
  auto &rawData = m_prototype[key];
  if (rawData.ends_with(data)) {
    rawData.erase(rawData.find(data) - 1, data.length());
  } else {
    rawData += L"," + data;
  }

  m_prototype[key] = rawData;
}

void Control::object::ToggleFlags(ObjectFlagID flag) {
  allObjectFlags[this] = allObjectFlags[this] ^ flag;
}

bool Control::object::HasFlags(ObjectFlagID flag) {
  return (allObjectFlags[this] & flag) != NULL;
}

bool Control::object::_OnPaint(Graphics &) { return false; }

bool Control::object::EventPrcessor(UINT uMsg, WPARAM wParam, LPARAM lParam) {

  if (uMsg == WM_CREATE) {
    auto data = (InitContext*)(void*)lParam;
    m_renderScreen == data->RenderTarget;
  }

  if (not m_bEnable or not IsVisible()) return false;

  if (uMsg == WM_MOUSEMOVE || uMsg == WM_MOUSELEAVE) {
    m_isMouseHover = HasFlags(MouseHover);
    // return EventPrcessor();
  }

  if (uMsg == WM_LBUTTONDOWN) {
    return m_isLeftBtnDown = true;
  }

  if (uMsg == WM_LBUTTONUP) {
    if (not m_isLeftBtnDown) return false;

    m_isLeftBtnDown = false;

    CallAllEventHandlers(EventHandlerID::Click);
  }

  if (uMsg == WM_PAINT) {
    auto renderContext = (RenderContext *)(void *)lParam;
    RectF::Union(*renderContext->presentRect, *renderContext->presentRect,
                 m_rcObject);
    return _OnPaint(*renderContext->graphics);
  }
}