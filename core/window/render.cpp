#include "./render.h"

static WindowProp windowProps{};

static HWND hWindow{};
static ULONG_PTR gdipToken{};
static UnitTree* RenderTree{};
static DoubleBuffer* RenderContext{};
static bool bProcess = false;

bool WindowRender::Processed() {
  if (bProcess)
    return !(bProcess = false);

  return false;
}

static void FlushScreen(bool force = true);
static void SendMessageUnitEx(INodeProp* lpNode,
                              UINT uMsg,
                              WPARAM wParam,
                              LPARAM lParam);
static void SendMessageUnit(UINT uMsg, WPARAM wParam, LPARAM lParam);
void WindowEntry(HWND);
void WindowExit();
LRESULT __stdcall WindowRender::WindowProcess(HWND hWnd,
                                              UINT uMsg,
                                              WPARAM wParam,
                                              LPARAM lParam) {
  static bool isAllInited = false;
  if (uMsg == WM_CREATE) {
    WindowEntry(hWnd);
    isAllInited = true;
  }

  if (uMsg == WM_MOVE) {
    windowProps.position = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
    RenderContext->FlushPos(windowProps.position);
  }

  if (uMsg == WM_SIZE) {
    windowProps.size = {LOWORD(lParam), HIWORD(lParam)};
    RenderContext->FlushSize(windowProps.size);
  }

  if (uMsg == WM_SETFOCUS or uMsg == WM_KILLFOCUS) {
    windowProps.IsFocused = uMsg == WM_SETFOCUS;
  }

  if (uMsg == WM_CLOSE) {
    DestroyWindow(hWnd);
    bProcess = true;
    return NULL;
  }

  if (uMsg == WM_DESTROY) {
    PostQuitMessage(0);
    bProcess = true;
    return NULL;
  }

  if (uMsg == WM_CHAR) {
    {
      // FlushScreen();
    }
  }

  if (isAllInited) {
    if (/*Mouse Event*/ (WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST) ||
        /*Keyboard Event*/ (uMsg == WM_CHAR || uMsg == WM_KEYDOWN ||
                            uMsg == WM_KEYUP)) {
      SendMessageUnit(uMsg, wParam, lParam);
      FlushScreen();

      if (uMsg == WM_CHAR || uMsg == WM_KEYDOWN || uMsg == WM_KEYUP)
        FlushScreen(true);
    }
  }

  return NULL;
}

void WindowEntry(HWND hWnd) {
  hWindow = hWnd;
  GdiplusStartupInput gdiplusStartupInput{};
  GdiplusStartup(&gdipToken, &gdiplusStartupInput, nullptr);

  if (Environment::GetEnv().EnableBlur) {
    Extension::BlurHelper::EnableBlur(hWnd);
  }

  RenderTree = new UnitTree();
  RenderContext = new DoubleBuffer(hWnd);

  RenderManager* mnr = new RenderManager(hWnd, RenderTree);
  RenderTree->GetOwner()->rcNode = Rect(0, 0, 800, 600);
  RenderTree->GetOwner()->ptrUnit = mnr;

  mnr->_handler(WM_CREATE, NULL, (LPARAM) & (RenderTree->GetOwner()->rcNode));

  InputContainer* child = new InputContainer(hWindow, RenderTree);
  auto prop = new INodeProp;
  prop->ptrUnit = child;
  prop->rcNode = Rect(100, 100, 100, 50);
  child->SetParent(mnr, prop);

  child->_handler(WM_CREATE, NULL, (LPARAM) & (prop->rcNode));

  FlushScreen();
}

void WindowExit() {
  delete RenderContext;
  RenderContext = nullptr;

  GdiplusShutdown(gdipToken);
};

static void FlushScreen(bool force) {
  Graphics graph(RenderContext->GetRenderDC());
  graph.SetSmoothingMode(SmoothingModeHighQuality);
  graph.Clear(Color::Transparent);
  
  if (force)
    SendMessageUnit(WM_PAINT, NULL, NULL);
  SendMessageUnit(WM_REPAINT, NULL, (LPARAM)&graph);

  RenderContext->Flush();
}

/*
  New
*/

#define ISMOUSEEVEENT(msg) (WM_MOUSEFIRST <= msg and msg <= WM_MOUSELAST)
#define ISKEYBOARDEVENT(msg) (msg == WM_CHAR)
#define ISKEYBOARDEVENT_OPT(msg) (msg == WM_KEYDOWN or msg == WM_KEYUP)

static bool _SendMessage(INodeProp* lpNode,
                         UINT uMsg,
                         WPARAM wParam,
                         LPARAM lParam) {
  static RenderUnit* lastFocusUnit{};

  if (lpNode == nullptr) {
    _SendMessage(RenderTree->GetOwner(), uMsg, wParam, lParam);
    return false;
  }

  if (not ISMOUSEEVEENT(uMsg)) {

    const auto selfPtr = (RenderUnit*)lpNode->ptrUnit;
    if (ISKEYBOARDEVENT(uMsg) or ISKEYBOARDEVENT_OPT(uMsg)) {
      if (selfPtr or (selfPtr->UnitType == UNIT_INPUT_COMPONENT &&
                      selfPtr == lastFocusUnit))
        selfPtr->_handler(uMsg, wParam, lParam);

    } else {
      selfPtr->_handler(uMsg, wParam, lParam);
    }

    if (lpNode->broUnit != nullptr) {
      _SendMessage(lpNode->broUnit, uMsg, wParam, lParam);
    }

    if (lpNode->childUnit != nullptr) {
      _SendMessage(lpNode->childUnit, uMsg, wParam, lParam);
    }

    return true;
  }

  const auto ptMouse = Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
  if (ISMOUSEEVEENT(uMsg)) {
    auto currNode = lpNode;

    while (currNode != nullptr) {
      auto isProcessed = false;
      if (currNode->childUnit) {
        isProcessed = _SendMessage(currNode->childUnit, uMsg, wParam, lParam);
      }

      if ((not currNode->rcNode.Contains(ptMouse)) or isProcessed) {
        currNode = currNode->broUnit;
        continue;
      }

      const auto finalUnit = (RenderUnit*)currNode->ptrUnit;

      if (uMsg == WM_LBUTTONDOWN) {
        if (lastFocusUnit)
          lastFocusUnit->_handler(WM_KILLFOCUS, NULL,
                                  (LPARAM)currNode->ptrUnit);
        finalUnit->_handler(WM_SETFOCUS, NULL, (LPARAM)lastFocusUnit);
        lastFocusUnit = finalUnit;
      }

      finalUnit->_handler(uMsg, wParam, lParam);
      return true;
    }
  }
}

static void SendMessageUnitEx(INodeProp* lpNode,
                              UINT uMsg,
                              WPARAM wParam,
                              LPARAM lParam) {
  _SendMessage(lpNode, uMsg, wParam, lParam);
}

static void SendMessageUnit(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  SendMessageUnitEx(nullptr, uMsg, wParam, lParam);
}
