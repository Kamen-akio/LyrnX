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
  classInfo.hInstance = hInstance;
  classInfo.lpfnWndProc = GlobalMessageHandler;
  classInfo.lpszClassName = L"kamen.minecraft.launcher";

  auto const hAtom = RegisterClassExW(&classInfo);
  if (hAtom == NULL) {
    throw std::exception("Where is your 'ATOM' !");
  }

  hMainWindow = hWnd = CreateWindowExW(
      0, L"kamen.minecraft.launcher", L"MinecraftLauncher", WS_OVERLAPPEDWINDOW,
      0, 0, 800, 600, NULL, NULL, hInstance, nullptr);

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

void _OnButtonClick() {
  MessageBoxW(hMainWindow, L"我被点击了🥵", L"受不了了", S_OK);
}

LRESULT Core::GlobalMessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam,
                                   LPARAM lParam) {
  static bool isInited = false;
  static bool isMouseTracked = false;
  static Control::screen* screen;
  static Control::button* test;

  if (uMsg == WM_CREATE) {
    screen = new Control::screen;
    screen->Create(hWnd);

    test = new Control::button;
    test->Create(RectF(100, 100, 200, 64));
    test->SetVisible(true);
    test->SetEnable(true);
    test->SetContext(L"6");
    test->SetClickHandler(&_OnButtonClick);

    screen->SetChild(test);

    _TrackMouse(hWnd);
    isMouseTracked = isInited = true;
    return 0;
  }

  if (isInited) {
    screen->EventPrcessor(uMsg, wParam, lParam);
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
    delete screen;
  }

  return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
