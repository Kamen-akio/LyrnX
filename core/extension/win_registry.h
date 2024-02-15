#pragma once
#include "../dependencies/process.h"

#ifndef _EXTENSION_WIN_REGISTRY_H_
#define _EXTENSION_WIN_REGISTRY_H_

namespace Extension {
namespace RegistryHelper {
class Registry {
 public:
  Registry(std::string path);
  ~Registry();

  void* GetItemPtr(std::string item);

 private:
  HKEY _root{};
  HKEY _item{};
};
}  // namespace RegistryHelper
}  // namespace Extension

#endif  // _EXTENSION_WIN_REGISTRY_H_