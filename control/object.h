#pragma once
#include <Windows.h>
#include <gdiplus.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "../core/renderManager.h"

using Core::CRenderManager;
using std::unordered_map;
using std::vector;
using std::wstring;

#ifndef _COMPONENT_OBJECT_
#define _COMPONENT_OBJECT_

#define WM_WNDSIZE WM_SIZE

namespace Component {
namespace PrototypeID {
typedef enum __tagPrototypeID {
  PrototypeIdGeneral_FontColor,
  PrototypeIdGeneral_BackgroundColor,
  PrototypeIdGeneral_BackgroundHoverColor,
  PrototypeIdGeneral_BackgroundActiveColor


} PrototypeID;
};
class Object {
 public:
  Object();
  ~Object();
  void Destroy();

 public:
  void SetPrototype(PrototypeID::PrototypeID, void*);
  void DelPrototype(PrototypeID::PrototypeID);
  void* GetPrototype(PrototypeID::PrototypeID);

  void InitObjectBase(CRenderManager* manager);
  LRESULT MessageProcess(UINT uMsg, WPARAM wParam, LPARAM lParam);

  virtual void Init(){};

 protected:
  CRenderManager* m_renderManager;

  unordered_map<PrototypeID::PrototypeID, void*> m_protoMap;
};
}  // namespace Component

#endif  // !_COMPONENT_OBJECT_
