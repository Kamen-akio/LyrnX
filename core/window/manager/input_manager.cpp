#include "input_manager.h"

InputContainer::InputContainer(HWND window, UnitTree* tree)
    : RenderUnit(window, tree) {}

void InputContainer::Render(Graphics* graph) {
  Font editFont(L"Segoe UI", 12);
  SolidBrush editBrush(Color::White);

  graph->Clear(Color::BlueViolet);
  graph->DrawString(context.c_str(), context.length(), &editFont,
                    RectF(0, 0, rcUnit.Width, rcUnit.Height), nullptr,
                    &editBrush);
}

static bool isEmoji(wchar_t value);
void InputContainer::GlobalEventHandler(UINT uMsg, WPARAM wp, LPARAM lp) {
  auto hContext = ImmGetContext(hWindow);

  if (not isFocus)
    return;

  if (uMsg == WM_CHAR) {
    std::wstring inputRaw(1, static_cast<wchar_t>(wp));

    /* Tab */

    if (inputRaw == L"\t")
      return;

    /* Enter */

    if (inputRaw == L"\r") {
      context += L"\r\n";
      currentPos++;
      return;
    }

    /* Espace*/

    if (inputRaw == L"\b") {
      if (not context.empty() || context.ends_with(L"\n")) {
        context.pop_back();
        currentPos--;
      }

      if (not context.empty() && context.ends_with(L"\r"))
        context.pop_back();

      if (not context.empty() && isEmoji(context.back()))
        context.pop_back();

      return;
    }

    /* Copy */

    if (inputRaw == L"\x3") {
      HGLOBAL hCopyChunk{};
      void* lpCopyChunk{};

      if (selectChunk.Empty())
        hCopyChunk = GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,
                                 (context.length() + 1) * 2);

      if (not hCopyChunk or lpCopyChunk == nullptr)
        return;

      lpCopyChunk = GlobalLock(hCopyChunk);
      memcpy(lpCopyChunk, context.data(), GlobalSize(hCopyChunk));
      GlobalLock(hCopyChunk);

      OpenClipboard(NULL);
      if (hCopyChunk)
        SetClipboardData(CF_UNICODETEXT, hCopyChunk);
      CloseClipboard();

      GlobalFree(hCopyChunk);
      return;
    }

    /* Paste */

    if (inputRaw == L"\x16") {
      OpenClipboard(NULL);

      auto hData = GetClipboardData(CF_UNICODETEXT);
      wchar_t* pasteContext = static_cast<wchar_t*>(GlobalLock(hData));

      if (pasteContext == nullptr) {
        CloseClipboard();
        return;
      }

      if (selectChunk.Empty())
        context += pasteContext;
      else {
        context.erase(selectChunk.position, selectChunk.size);
        context.insert(selectChunk.position, pasteContext);
      }

      GlobalUnlock(hData);
      CloseClipboard();
      return;
    }

    context += inputRaw;
    if (not isEmoji(context.back()))
      currentPos++;
    GetContextOfLine();
    return;
  }

  if (uMsg == WM_KEYDOWN) {
    if (wp == VK_HOME) {
      // ...
    }

    if (wp == VK_END) {
      // ...
    }
  }

  ImmReleaseContext(hWindow, hContext);
}

int InputContainer::GetCurrentPosition() const {
  return currentPos;
}

std::wstring InputContainer::GetContextOfLine() {
  size_t maxSize = context.length();
  size_t lineBegin = context.rfind(L"\r\n", currentPos),
         lineEnd = context.find(L"\r\n", lineBegin + 2);

  if (lineBegin == std::wstring::npos)
    return context;

  if (lineEnd == std::wstring::npos)
    lineEnd = maxSize;

  return context.substr(lineBegin + 2, lineEnd - lineBegin);
};

static bool isEmoji(wchar_t value) {
  return (value >= 0xd800 && value <= 0xdbff) or
         ((0x2100 <= value && value <= 0x27ff && value != 0x263b) ||
          (0x2b05 <= value && value <= 0x2b07) ||
          (0x2934 <= value && value <= 0x2935) ||
          (0x3297 <= value && value <= 0x3299) || value == 0xa9 ||
          value == 0xae || value == 0x303d || value == 0x3030 ||
          value == 0x2b55 || value == 0x2b1c || value == 0x2b1b ||
          value == 0x2b50 || value == 0x231a);
}