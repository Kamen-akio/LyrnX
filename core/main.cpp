#include "./main.h"

using namespace Gdiplus;

ULONG_PTR gdiplusToken{};

static HWND hMainWindow;

void Core::Init(HINSTANCE hInstance, int nCmdShow, HWND& hWnd) {
  GdiplusStartupInput startupInput{};
  GdiplusStartup(&gdiplusToken, &startupInput, nullptr);

  WNDCLASSEXW classInfo{};
  classInfo.cbSize = sizeof WNDCLASSEXW;
  classInfo.cbClsExtra = 0;
  classInfo.hCursor = LoadCursorW(NULL, IDC_ARROW);
  classInfo.hInstance = hInstance;
  classInfo.lpfnWndProc = GlobalMessageHandler;
  classInfo.lpszClassName = L"kamen.minecraft.launcher";

  auto const hAtom = RegisterClassExW(&classInfo);
  if (hAtom == NULL) {
    throw std::exception("Where is your 'ATOM' !");
  }

  hMainWindow = hWnd = CreateWindowExW(
      WS_EX_APPWINDOW, L"kamen.minecraft.launcher", L"",
      WS_OVERLAPPEDWINDOW, 0,
      0, 800, 600, NULL, NULL, hInstance, nullptr);

  ShowWindow(hWnd, nCmdShow);
}

void Core::Destroy() { GdiplusShutdown(gdiplusToken); }

inline void _TrackMouse(HWND hWnd) {
  TRACKMOUSEEVENT trackEvent{};
  trackEvent.cbSize = sizeof(TRACKMOUSEEVENT);
  trackEvent.dwFlags = TME_LEAVE;
  trackEvent.hwndTrack = hWnd;
  TrackMouseEvent(&trackEvent);
}

static void _OnButtonClick() {
  Beep(rand() % 0x7FFF, 1000);
  // MessageBoxW(hMainWindow, L"我被点击了🥵", L"受不了了", S_OK);
}

LRESULT Core::GlobalMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam,
                                   LPARAM lParam) {
  static bool isInited = false;
  static bool isMouseTracked = false;

  static Core::CRenderManager* renderManager;
  static Component::Screen screen{};
  static Component::Button button{};

  if (uMsg == WM_CREATE) {
    // Core::Dwmapi::ToggleRenderInNCPAINT(hWnd, true);
    // Core::Dwmapi::ToggleAnyEffectWindow(
    //     hWnd, Dwmapi::ACCENT_ENABLE_ACRYLICBLURBEHIND, true);
    // Core::Dwmapi::ExtendFrameIntoClientArea(hWnd);

    // auto test = Bitmap::FromFile(L"C:\\Users\\A Normal
    // User\\Desktop\\GJ5nsoDaQAAbjg9.jpg");

    renderManager = new Core::CRenderManager(hWnd);

    screen.Bind(hWnd);
    button.SetControlRect({0, 0, 100, 100});
    button.InitObjectBase(renderManager);
    screen.SetChild(&button);
    screen.InitObjectBase(renderManager);

    _TrackMouse(hWnd);
    isMouseTracked = isInited = true;
    // return 0;
  }

  auto result = screen.MessageProcess(uMsg, wParam, lParam);
  if (screen.IsProcessed()) {
    return result;
  }

  if (uMsg == WM_MOUSEMOVE) {
    if (not isMouseTracked) {
      _TrackMouse(hWnd);
      isMouseTracked = true;
    }
  }

  if (uMsg == WM_MOUSELEAVE) {
    isMouseTracked = false;
    return NULL;
  }

  if (uMsg == WM_ERASEBKGND) {
    return TRUE;
  }

  if (uMsg == WM_CLOSE) {
    DestroyWindow(hWnd);
    PostQuitMessage(NULL);
    return 0;
  }

  if (uMsg == WM_DESTROY) {
    delete renderManager;
    // delete screen;
  }

  return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
