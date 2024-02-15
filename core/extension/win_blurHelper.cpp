#include "win_blurHelper.h"
#include "win_registry.h"


using namespace Extension::Undocument;

void Extension::BlurHelper::EnableBlur(HWND hWnd, BlurFlag type) {
  auto buildTime = Extension::System::GetSystemBuildtime();
  bool isWin11 = buildTime >= 22000;

  if (!isWin11 and (type == Mica || type == Acrylic || type == Auto))
    type = Aero;

  if (isWin11 and type == Auto) {
    if (buildTime >= 22621)
      type = Mica;
    else
      type = Acrylic;
  }

  auto hUser32 = GetModuleHandleA("user32.dll");
  if (hUser32 == NULL)
    hUser32 = LoadLibraryA("user32.dll");

  auto SetWindowCompositionAttribute =
      (pfnSetWindowCompositionAttribute)GetProcAddress(
          hUser32, "SetWindowCompositionAttribute");

  if (!SetWindowCompositionAttribute)
    return (void)(FreeLibrary(hUser32));

  ACCENT_POLICY accent = {(DWORD)type, 0, 0, 0};
  WINDOWCOMPOSITIONATTRIBDATA attrib{};
  attrib.cbData = sizeof(WINDOWCOMPOSITIONATTRIBDATA);
  attrib.Attrib = 19;  // WCA_ACCENT_POLICY
  attrib.pvData = &accent;

  SetWindowCompositionAttribute(hWnd, &attrib);
  if (hUser32 != NULL)
    FreeLibrary(hUser32);
}

int Extension::System::GetSystemBuildtime() {
  RegistryHelper::Registry registry(
      "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion");
  char* ptVersion = (char*)registry.GetItemPtr("CurrentBuildNumber");
  std::string strVersion(ptVersion);
  std::istringstream iss(strVersion);
  int buildVersion{};
  iss >> buildVersion;
  free(ptVersion);
  return buildVersion;
}