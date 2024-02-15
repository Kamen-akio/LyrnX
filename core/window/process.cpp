#include "./process.h"

static bool bProcess = false;

bool WindowProcess::Processed() {
  if (bProcess)
    return !(bProcess = false);

  return false;
}

void WindowEntry();

LRESULT __stdcall WindowProcess::WindowProcess(HWND hWnd,
                                               UINT uMsg,
                                               WPARAM wParam,
                                               LPARAM lParam) {
  if (uMsg == WM_CREATE) {
    WindowEntry();
  }
  return NULL;
}

void WindowEntry() {

  // Todo.

}