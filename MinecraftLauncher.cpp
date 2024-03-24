#include "./MinecraftLauncher.h"

#include "./core/core.h"
#include "./core/impl/gdipDoubleBuffer.h"
#include "./core/impl/multiwayTree.h"

HWND hMainWindow{};
HINSTANCE hProcessInstance{};

static void test(void* data) {
  if (true) {
    cos(1);
  }
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  hInstance = NULL;
  hMainWindow = NULL;

  hProcessInstance = hInstance;
  Core::Init(hInstance, nCmdShow, hMainWindow);

  MSG msg{};
  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }

  return 0;
}
