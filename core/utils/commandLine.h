#pragma once
#include "../dependencies/process.h"

#include <processenv.h>
#include <unordered_map>

#ifndef _UTILS__COMMANDLINE_H_
#define _UTILS__COMMANDLINE_H_

namespace CommandLineHelper {

void Init(std::wstring commandLine = L"");
void Destroy();

std::unordered_map<std::wstring, std::vector<std::wstring>> GetCommandLineMap();
std::vector<std::wstring> GetProperties(std::wstring item);
bool IsItemExist(std::wstring item);
bool IsPropertyExist(std::wstring item, std::wstring property);

}  // namespace CommandLineHelper

#endif  // !_UTILS__COMMANDLINE_H_
