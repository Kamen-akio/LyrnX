#include "input_manager.h"

InputContainer::InputContainer(HWND window, UnitTree* tree)
    : RenderUnit(window, tree) {}

void InputContainer::Render(Graphics* graph) {
  graph->Clear(Color::BlueViolet);
}

void InputContainer::GlobalEventHandler(UINT uMsg, WPARAM wp, LPARAM lp) {
  auto hContext = ImmGetContext(hWindow);

  if (not isFocus)
    return;

  if (uMsg == WM_CHAR) {
    inputData += static_cast<wchar_t>(wp);

    // detect is empty

    if (inputData._Equal(L"\b"))
      return inputData.pop_back();

    // - - -

    if (inputData.ends_with(L"\t") || inputData.ends_with(L"\r"))
      inputData.pop_back();

    if (inputData.ends_with(L"\b") && inputData.size() > 1)
      inputData.erase(inputData.size() - 2, 2);

    if (inputData.ends_with(L"\r"))
      inputData += L"\r\n";

    return;
  }

  ImmReleaseContext(hWindow, hContext);
};
