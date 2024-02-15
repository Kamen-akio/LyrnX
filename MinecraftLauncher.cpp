
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS

#include "./core/dependencies/targetver.h"

#include "./core/dependencies/process.h"

#include "./core/utils/environment.h"
#include "./core/window/process.h"
#include "./core/window/render.h"

#include "./core/extension/win_blurHelper.h"

extern HWND hLauncherWnd{};
extern HINSTANCE hLauncherInstance{};

static std::vector<std::wstring> wc_split(const wchar_t* in,
                                          const wchar_t* delim);

bool InitialWindow(int);
bool InitialProgram(std::wstring);
LRESULT __stdcall WindowMessageProc(HWND hWnd,
                                    UINT uMsg,
                                    WPARAM wParam,
                                    LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  hLauncherInstance = hInstance;

  IS_FAIL(InitialProgram(lpCmdLine));
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

static void CmdLine_ProcessFlag(std::wstring data);
bool InitialProgram(std::wstring cmdLine) {
  auto cmdLineUnit =
      wc_split(cmdLine.c_str(),
               L"/[\\/-]?((\\w+)(?:[=:](\"[^\"]+\"|[^\\s\"]+))?)(?:\\s+|$)/g");
  for (auto& item : cmdLineUnit) {
    if (item.starts_with(L"/flag=")) {
      CmdLine_ProcessFlag(item);
    }
  }
  return true;
}

static void CmdLine_ProcessFlag(std::wstring data) {
  data = data.substr(strlen("/flag="));
  const auto flags = wc_split(data.c_str(), L",");

  for (auto& flag : flags) {
    if (flag == L"enable_debug") {
      Environment::GetEnvRaw()->EnableDebug = true;
    }

    if (flag == L"enable_blur") {
      Environment::GetEnvRaw()->EnableBlur = true;
    }

    if (flag == L"enable_rounded_frame") {
      Environment::GetEnvRaw()->EnableRoundedFrame = true;
    }
  }

  auto b = Environment::GetEnvRaw();
}

bool InitialWindow(int ShowType) {
  WNDCLASSEXW classInfo{};
  classInfo.cbSize = sizeof(WNDCLASSEXW);
  classInfo.hInstance = hLauncherInstance;
  classInfo.lpfnWndProc = WindowMessageProc;
  classInfo.lpszClassName = L"lyrnx.minecraft.launcher";

  IS_FAIL(RegisterClassExW(&classInfo));

  hLauncherWnd = CreateWindowExW(WS_EX_LAYERED, classInfo.lpszClassName,
                                 L"LyrnX.Minecraft.Launcher", WS_VISIBLE | WS_POPUP,
                                 CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL,
                                 NULL, hLauncherInstance, nullptr);
  ShowWindow(hLauncherWnd, ShowType);
  ;
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

// string.

static std::vector<std::wstring> wc_split(const wchar_t* in,
                                          const wchar_t* delim) {
  std::wregex re{delim};
  return std::vector<std::wstring>{
      std::wcregex_token_iterator(in, in + wcslen(in), re, -1),
      std::wcregex_token_iterator()};
}