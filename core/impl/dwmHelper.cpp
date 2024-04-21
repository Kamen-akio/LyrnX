#include "dwmHelper.h"

bool Core::Dwmapi::ToggleRenderInNCPAINT(HWND hWnd, bool toggle) {
  // DWMNCRP_DISABLED   -   0x01
  // DWMNCRP_ENABLED    -   0x02
  auto status = DWMNCRP_DISABLED + (int)toggle;

  auto result = DwmSetWindowAttribute(hWnd, DWMWA_NCRENDERING_POLICY, &status,
                                      sizeof(status));

  return SUCCEEDED(result);
}

bool Core::Dwmapi::ToggleAnyEffectWindow(HWND hWnd, ACCENTSTATE flag,
                                         bool toggle) {
  auto hUser32 = LoadLibrary(L"user32.dll");
  if (hUser32 == nullptr) {
    return false;
  }

  auto pFunc = GetProcAddress(hUser32, "SetWindowCompositionAttribute");
  if (pFunc == nullptr) {
    FreeLibrary(hUser32);
    return false;
  }

  ACCENTPOLICY policy{};
  policy.GradientColor = 0xF0010101;
  policy.AccentState = toggle ? flag : ACCENT_DISABLED;
  policy.AccentFlags = 2;

  WINDOWCOMPOSITIONATTRIBDATA data{};
  data.Attrib = WCA_ACCENT_POLICY;
  data.cbData = sizeof(policy);
  data.pvData = &policy;

  auto rs = ((__implSetWindowCompositionAttribute)pFunc)(hWnd, &data);

  FreeLibrary(hUser32);
  return rs;
}

bool Core::Dwmapi::ToggleBlurBehindWindow(HWND hWnd, bool toggle) {
  DWM_BLURBEHIND bh{};
  bh.dwFlags = DWM_BB_ENABLE;
  bh.fEnable = toggle;

  return SUCCEEDED(DwmEnableBlurBehindWindow(hWnd, &bh));
}

bool Core::Dwmapi::ExtendFrameIntoClientArea(HWND hWnd, MARGINS rect) {
  auto localRect = rect;
  return SUCCEEDED(DwmExtendFrameIntoClientArea(hWnd, &localRect));
}
