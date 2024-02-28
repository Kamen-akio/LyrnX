
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "./core/dependencies/targetver.h"

#include "./core/dependencies/process.h"

#include "./core/utils/commandLine.h"
#include "./core/utils/environment.h"
#include "./core/window/process.h"
#include "./core/window/render.h"

#include "./core/extension/win_blurHelper.h"

HWND hLauncherWnd{};
HINSTANCE hLauncherInstance{};

void ProcessFlag();
bool InitialWindow(int);
LRESULT __stdcall WindowMessageProc(HWND hWnd,
                                    UINT uMsg,
                                    WPARAM wParam,
                                    LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);

  hLauncherInstance = hInstance;
  CommandLineHelper::Init(lpCmdLine);

  ProcessFlag();
  IS_FAIL(InitialWindow(nCmdShow));

  MSG msg{};
  while (GetMessage(&msg, 0, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
}

/*
 * /// Todo, for future.
 */

static void ProcessFlag() {
  using namespace CommandLineHelper;

  Environment::GetEnvRaw()->EnableDebug =
      IsPropertyExist(L"flag", L"enable_debug");

  Environment::GetEnvRaw()->EnableBlur =
      IsPropertyExist(L"flag", L"enable_blur");

  Environment::GetEnvRaw()->EnableRoundedFrame =
      IsPropertyExist(L"flag", L"enable_rounded_frame");

  Environment::GetEnvRaw()->EnableRoundedFrame =
      IsPropertyExist(L"flag", L"enable_rounded_frame");

  Environment::GetEnvRaw()->EnableMacNewline =
      IsPropertyExist(L"flag", L"enable_mac_newline");

  Environment::GetEnvRaw()->EnableUnixNewline =
      IsPropertyExist(L"flag", L"enable_unix_newline");
}

bool InitialWindow(int ShowType) {
  WNDCLASSEXW classInfo{};
  classInfo.cbSize = sizeof(WNDCLASSEXW);
  classInfo.hInstance = hLauncherInstance;
  classInfo.lpfnWndProc = WindowMessageProc;
  classInfo.lpszClassName = L"lyrnx.minecraft.launcher";

  IS_FAIL(RegisterClassExW(&classInfo));

  hLauncherWnd = CreateWindowExW(
      WS_EX_LAYERED, classInfo.lpszClassName, L"LyrnX.Minecraft.Launcher",
      WS_VISIBLE | WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL,
      hLauncherInstance, nullptr);
  ShowWindow(hLauncherWnd, ShowType);
  return !!hLauncherWnd;
}

LRESULT __stdcall WindowMessageProc(HWND hWnd,
                                    UINT uMsg,
                                    WPARAM wParam,
                                    LPARAM lParam) {
  {
    LRESULT rsProc = WindowRender::WindowProcess(hWnd, uMsg, wParam, lParam);
    if (WindowRender::Processed())
      return rsProc;
  }
  {
    LRESULT rsProc = WindowProcess::WindowProcess(hWnd, uMsg, wParam, lParam);
    if (WindowProcess::Processed())
      return rsProc;
  }
  return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}
