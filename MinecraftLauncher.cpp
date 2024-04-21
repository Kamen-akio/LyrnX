#include "./MinecraftLauncher.h"

#include "./core/core.h"
#include "./core/impl/gdipExt.h"
#include "./core/impl/multiwayTree.h"

HWND hMainWindow{};
HINSTANCE hProcessInstance{};

static void __DEBUG_FUNCTION__() {
  // auto c = Gdiplus::GetColorFromStringA("FFFFFF");
  // DebugBreak();
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
#ifdef _DEBUG
  __DEBUG_FUNCTION__();
#endif  // DEBUG

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
