#pragma once
#include "../dependencies/process.h"

namespace Extension {

namespace Undocument {
typedef struct __tagACCENT_POLICY {
  DWORD AccentState;
  DWORD AccentFlags;
  DWORD GradientColor;
  DWORD AnimationId;
} ACCENT_POLICY;
typedef struct __tagWINDOWCOMPOSITIONATTRIBDATA {
  int Attrib;
  void* pvData;
  int cbData;
} WINDOWCOMPOSITIONATTRIBDATA;
typedef BOOL(WINAPI* pfnSetWindowCompositionAttribute)(
    HWND,
    WINDOWCOMPOSITIONATTRIBDATA*);
}  // namespace Undocument

namespace System {
int GetSystemBuildtime();
}

namespace BlurHelper {

typedef enum __tagBlurFlag { Auto = -1, Aero = 3, Acrylic, Mica } BlurFlag;
void EnableBlur(HWND, BlurFlag type = Auto);

}  // namespace BlurHelper

}  // namespace Extension