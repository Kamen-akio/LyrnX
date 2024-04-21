#pragma once
#include <dwmapi.h>
#include <windows.h>
#pragma comment(lib, "dwmapi.lib")

#ifndef _IMPL_DWMHELPER_H_
#define _IMPL_DWMHELPER_H_

namespace Core {
namespace Dwmapi {

typedef enum __tagACCENTSTATE {
  ACCENT_DISABLED = 0,
  ACCENT_ENABLE_GRADIENT = 1,
  ACCENT_ENABLE_TRANSPARENTGRADIENT = 2,
  ACCENT_ENABLE_BLURBEHIND = 3,
  ACCENT_ENABLE_ACRYLICBLURBEHIND = 4,
  ACCENT_INVALID_STATE = 5
} ACCENTSTATE;

typedef struct _tagACCENTPOLICY {
  ACCENTSTATE AccentState;
  DWORD AccentFlags;
  DWORD GradientColor;
  DWORD AnimationId;
} ACCENTPOLICY;

typedef enum __tagWINDOWCOMPOSITIONATTRIB {
  WCA_UNDEFINED = 0x0,
  WCA_NCRENDERING_ENABLED = 0x1,
  WCA_NCRENDERING_POLICY = 0x2,
  WCA_TRANSITIONS_FORCEDISABLED = 0x3,
  WCA_ALLOW_NCPAINT = 0x4,
  WCA_CAPTION_BUTTON_BOUNDS = 0x5,
  WCA_NONCLIENT_RTL_LAYOUT = 0x6,
  WCA_FORCE_ICONIC_REPRESENTATION = 0x7,
  WCA_EXTENDED_FRAME_BOUNDS = 0x8,
  WCA_HAS_ICONIC_BITMAP = 0x9,
  WCA_THEME_ATTRIBUTES = 0xA,
  WCA_NCRENDERING_EXILED = 0xB,
  WCA_NCADORNMENTINFO = 0xC,
  WCA_EXCLUDED_FROM_LIVEPREVIEW = 0xD,
  WCA_VIDEO_OVERLAY_ACTIVE = 0xE,
  WCA_FORCE_ACTIVEWINDOW_APPEARANCE = 0xF,
  WCA_DISALLOW_PEEK = 0x10,
  WCA_CLOAK = 0x11,
  WCA_CLOAKED = 0x12,
  WCA_ACCENT_POLICY = 0x13,
  WCA_FREEZE_REPRESENTATION = 0x14,
  WCA_EVER_UNCLOAKED = 0x15,
  WCA_VISUAL_OWNER = 0x16,
  WCA_HOLOGRAPHIC = 0x17,
  WCA_EXCLUDED_FROM_DDA = 0x18,
  WCA_PASSIVEUPDATEMODE = 0x19,
  WCA_LAST = 0x1A,
} WINDOWCOMPOSITIONATTRIB,
    WCA;

typedef struct __tagWINDOWCOMPOSITIONATTRIBDATA {
  WINDOWCOMPOSITIONATTRIB Attrib;
  void* pvData;
  DWORD cbData;
} WINDOWCOMPOSITIONATTRIBDATA, WCAD;

typedef BOOL(WINAPI* __implSetWindowCompositionAttribute)(
    IN HWND hwnd, IN WINDOWCOMPOSITIONATTRIBDATA* pwcad);

bool ToggleRenderInNCPAINT(HWND hWnd, bool toggle);

#pragma message( \
    "::Core::Dwmapi [WARN] ToggleAnyEffectWindow Api may be unable to work in the future. ")
bool ToggleAnyEffectWindow(HWND hWnd, ACCENTSTATE flag, bool toggle);
bool ToggleBlurBehindWindow(HWND hWnd, bool toggle);

bool ExtendFrameIntoClientArea(HWND hWnd, MARGINS rect = {-1});

};  // namespace Dwmapi
};  // namespace Core

#endif  // !_IMPL_DWMHELPER_H_
