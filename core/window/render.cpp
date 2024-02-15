#include "./render.h"

static WindowProp windowProps{};

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
static void CallAllUnitHandler(UINT uMsg, WPARAM wParam, LPARAM lParam);
static void CallUnitHandlerAsEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
void WindowEntry(HWND);
void WindowExit();
LRESULT __stdcall WindowRender::WindowProcess(HWND hWnd,
                                              UINT uMsg,
                                              WPARAM wParam,
                                              LPARAM lParam) {
  if (uMsg == WM_CREATE) {
    WindowEntry(hWnd);
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
      FlushScreen();
    }
  }

  if (WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST) {
    CallUnitHandlerAsEvent(uMsg, wParam, lParam);
  }

  return NULL;
}

class RenderManager : public RenderUnit {
 public:
  RenderManager(UnitTree* tree) : RenderUnit(tree){};

  void Render(Graphics* g) {

    SolidBrush bgBrush(isLeftBtnDown ? Color::AliceBlue : Color::BurlyWood);
    if (useRounded) {
      const int radius = 32;
      PathExt roundedPath{};

      roundedPath.AddBezierRounded(radius, rcUnit.X, rcUnit.Y, rcUnit.Width,
                                   rcUnit.Height);
      g->FillPath(&bgBrush, &roundedPath);
    } else
      g->FillRectangle(&bgBrush, rcUnit);

    Font font(L"Microsoft YaHei", 24);
    std::wstring context(isLeftBtnDown ? L"Clicked" : L"Click Me");
    SolidBrush fontBrush(Color::Green);
    auto a = g->DrawString(context.c_str(), context.length(), &font,
                  PointF(10, 10), &fontBrush);
  }

 public:
  bool useRounded = Environment::GetEnv().EnableRoundedFrame;
};

class Test : public RenderUnit {
 public:
  Test(UnitTree* tree) : RenderUnit(tree){};

  void Render(Graphics* g) {
    g->SetSmoothingMode(SmoothingModeHighQuality);

    g->Clear(isLeftBtnDown ? Color::Yellow : Color::Blue);

    Font font(L"Microsoft YaHei", 16);
    std::wstring context(isLeftBtnDown ? L"Clicked" : L"Click Me");
    SolidBrush fontBrush(Color::Green);
    auto a = g->DrawString(context.c_str(), context.length(), &font,
                           PointF(10, 10), &fontBrush);
  }
};

void WindowEntry(HWND hWnd) {
  GdiplusStartupInput gdiplusStartupInput{};
  GdiplusStartup(&gdipToken, &gdiplusStartupInput, nullptr);

  if (Environment::GetEnv().EnableBlur) {
    Extension::BlurHelper::EnableBlur(hWnd);
  }

  RenderTree = new UnitTree();
  RenderContext = new DoubleBuffer(hWnd);

  RenderManager* mnr = new RenderManager(RenderTree);
  RenderTree->GetOwner()->rcNode = Rect(0, 0, 800, 600);
  RenderTree->GetOwner()->ptrUnit = mnr;

  mnr->_handler(WM_CREATE_UNIT, NULL,
                (LPARAM) & (RenderTree->GetOwner()->rcNode));

  Test* child = new Test(RenderTree);
  auto prop = new INodeProp;
  prop->ptrUnit = child;
  prop->rcNode = Rect(100, 100, 100, 50);
  child->SetParent(mnr, prop);

  child->_handler(WM_CREATE_UNIT, NULL, (LPARAM) & (prop->rcNode));

  FlushScreen();
}

void WindowExit() {
  delete RenderContext;
  RenderContext = nullptr;

  GdiplusShutdown(gdipToken);
};

static void FlushScreen(bool force) {
  Graphics g(RenderContext->GetRenderDC());
  g.SetSmoothingMode(SmoothingModeHighQuality);
  g.Clear(Color::Transparent);

  if (force)
    CallAllUnitHandler(WM_REPAINT_UNIT, NULL, NULL);
  CallAllUnitHandler(WM_PAINT, NULL, (LPARAM)&g);

  RenderContext->Flush();
}

static void _callAll(INodeProp* node, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  if (node->ptrUnit) {
    ((RenderUnit*)(node->ptrUnit))->_handler(uMsg, wParam, lParam);

  if (node->broUnit)
    _callAll(node->broUnit, uMsg, wParam, lParam);

  if (node->childUnit)
    _callAll(node->childUnit, uMsg, wParam, lParam);

  }
}
static void CallAllUnitHandler(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  _callAll(RenderTree->GetOwner(), uMsg, wParam, lParam);
}

static bool _callEvent(INodeProp* node,
                       UINT uMsg,
                       WPARAM wParam,
                       LPARAM lParam) {
  const auto ptMouse = Point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

  bool isProc = false;
  auto currentNode = node;
  if (WM_MOUSEFIRST <= uMsg && uMsg <= WM_MOUSELAST) {
    while (currentNode != nullptr) {

      if (currentNode->childUnit) {
        isProc = _callEvent(currentNode->childUnit, uMsg, wParam, lParam);
      }

      if (currentNode->rcNode.Contains(ptMouse) && !isProc) {
        ((RenderUnit*)currentNode->ptrUnit)->_handler(uMsg, wParam, lParam);
        return true;
      }
      currentNode = currentNode->broUnit;
    }
  }
}
static void CallUnitHandlerAsEvent(UINT uMsg, WPARAM wParam, LPARAM lParam) {
  _callEvent(RenderTree->GetOwner(), uMsg, wParam, lParam);
  FlushScreen();
}
