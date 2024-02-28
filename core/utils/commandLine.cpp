#include "commandLine.h"

using namespace std;

static wstring commandLine{};
static unordered_map<wstring, vector<wstring>> commandStack{};

static vector<wstring> wsplit(wstring, TCHAR);
void CommandLineHelper::Init(wstring cmdLine) {
  commandLine = cmdLine = cmdLine.empty() ? GetCommandLineW() : cmdLine;

  int argLen = 0;
  wchar_t** argList = CommandLineToArgvW(cmdLine.c_str(), &argLen);

  for (int idx = 0; idx < argLen; idx++) {
    auto arg = wstring(argList[idx]);

    if (not arg.starts_with(L"/"))
      continue;

    arg.erase(arg.begin());

    auto symbolIdx = arg.find(L"=");
    if (symbolIdx == wstring::npos) {
      commandStack.emplace(arg, vector<wstring>());
      continue;
    }

    auto item = arg.substr(0, symbolIdx);
    auto props = wsplit(arg.substr(symbolIdx + 1), L',');
    commandStack.emplace(item, props);
  }
}

void CommandLineHelper::Destroy() {
  commandLine = L"";
}

unordered_map<wstring, vector<wstring>> CommandLineHelper::GetCommandLineMap() {
  return commandStack;
}

vector<wstring> CommandLineHelper::GetProperties(wstring item) {
  if (IsItemExist(item))
    return commandStack.find(item)->second;

  return vector<wstring>();
}

bool CommandLineHelper::IsItemExist(wstring item) {
  return commandStack.find(item) != commandStack.end();
}

bool CommandLineHelper::IsPropertyExist(wstring item, wstring property) {
  if (not IsItemExist(item))
    return false;

  for (auto& prop : GetProperties(item)) {
    if (prop.find(property) != wstring::npos)
      return true;
  }
  return false;
}

static vector<wstring> wsplit(wstring s, TCHAR delim) {
  wstring item;
  wstringstream wss;
  vector<wstring> elements;

  wss.str(s);
  while (getline(wss, item, delim)) {
    elements.push_back(item);
  }

  return elements;
}
