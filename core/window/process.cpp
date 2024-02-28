#include "./process.h"

static bool bProcess = false;

bool WindowProcess::Processed() {
  if (bProcess)
    return !(bProcess = false);

  return false;
}

void WindowEntry();
static LRESULT WindowHitTest(POINT posWnd, SIZE szWnd, POINT ptMouse);

LRESULT __stdcall WindowProcess::WindowProcess(HWND hWnd,
                                               UINT uMsg,
                                               WPARAM wParam,
                                               LPARAM lParam) {
  static bool bMouseIn{};
  static SIZE szWnd{};
  static POINT posWnd{};

  if (uMsg == WM_CREATE) {
    WindowEntry();
  }

  if (uMsg == WM_MOVE) {
    if (not bMouseIn) {
      bMouseIn = true;

      TRACKMOUSEEVENT mouseEvent{};
      mouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
      mouseEvent.dwFlags = TME_LEAVE;
      mouseEvent.hwndTrack = hWnd;
      mouseEvent.dwHoverTime = NULL;
      TrackMouseEvent(&mouseEvent);
    }

    posWnd = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
  }

  if (uMsg == WM_SIZE) {
    szWnd = {LOWORD(lParam), HIWORD(lParam)};
  }

  if (uMsg == WM_NCHITTEST) {
    return WindowHitTest(posWnd, szWnd,
                         {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)});
  }

  if (uMsg == WM_MOUSELEAVE) {
    bMouseIn = false;
  }

  return NULL;
}

void WindowEntry() {
  // Todo.
}

static LRESULT WindowHitTest(POINT posWnd, SIZE szWnd, POINT ptMouse) {
  return HTNOWHERE;
}