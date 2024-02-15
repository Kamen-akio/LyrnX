#include "win_registry.h"

Extension::RegistryHelper::Registry::Registry(std::string path) {
  if (path.starts_with("HKEY_CLASSES_ROOT"))
    _root = HKEY_CLASSES_ROOT;

  if (path.starts_with("HKEY_CURRENT_USER"))
    _root = HKEY_CURRENT_USER;

  if (path.starts_with("HKEY_LOCAL_MACHINE"))
    _root = HKEY_LOCAL_MACHINE;

  if (path.starts_with("HKEY_USERS"))
    _root = HKEY_USERS;

  if (path.starts_with("HKEY_CURRENT_CONFIG"))
    _root = HKEY_CURRENT_CONFIG;

  if (path.find("/") != std::string::npos)
    path = path.substr(path.find("/") + 1);

  if (path.find("\\") != std::string::npos)
    path = path.substr(path.find("\\") + 1);

  auto result =
      RegOpenKeyExA(_root, path.c_str(), NULL, KEY_QUERY_VALUE, &_item);
  if (result != ERROR_SUCCESS) {
    throw new std::exception("Open key fail!");
  }
};

void* Extension::RegistryHelper::Registry::GetItemPtr(std::string item) {
  ULONG itemType{};
  ULONG itemLength{};
  auto result = RegGetValueA(_item, NULL, item.c_str(),
                             RRF_RT_ANY, &itemType,
                             nullptr, &itemLength);

  if (result != ERROR_SUCCESS)
    return nullptr;

  void* itemPtr = malloc(itemLength);
  result = RegGetValueA(_item, NULL, item.c_str(),
                             RRF_RT_ANY | RRF_SUBKEY_WOW6432KEY, nullptr,
                             itemPtr, &itemLength);

  if (result != ERROR_SUCCESS) {
    free(itemPtr);
    return nullptr;
  }

  return itemPtr;
}

Extension::RegistryHelper::Registry::~Registry() {
  RegCloseKey(_item);
  _item = NULL;
}