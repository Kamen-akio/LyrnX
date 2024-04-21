#include "Object.h"
using namespace Gdiplus;

Component::Object::Object() {
  m_renderManager = nullptr;
  m_protoMap = unordered_map<PrototypeID::PrototypeID, void*>();
}

void Component::Object::Destroy() {
  m_protoMap.clear();
  delete this;
}

void Component::Object::SetPrototype(PrototypeID::PrototypeID id, void* pData) {
  m_protoMap[id] = pData;
}

void Component::Object::DelPrototype(PrototypeID::PrototypeID id) {
  delete m_protoMap[id];
  m_protoMap[id] = nullptr;
  m_protoMap.erase(id);
}

void* Component::Object::GetPrototype(PrototypeID::PrototypeID id) {
  return m_protoMap[id];
}

Component::Object::~Object() {
  // may nothing to do.
  for (auto& ptr : m_protoMap) {
    delete ptr.second;
    ptr.second = nullptr;
  }

  m_protoMap.clear();
}

void Component::Object::InitObjectBase(CRenderManager* manager) {
  m_renderManager = manager;
}

LRESULT Component::Object::MessageProcess(UINT uMsg, WPARAM wParam,
                                          LPARAM lParam) {
  return FALSE;
}
