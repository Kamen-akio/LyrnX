#pragma once
#include "../dependencies/render.h"

#ifndef _DEPEND_NODE_H_
#define _DEPEND_NODE_H_

struct IUnitProp {
  Rect rcNode{};
};

struct INodeProp : IUnitProp {
  void* ptrUnit{};
  INodeProp* broUnit{};
  INodeProp* childUnit{};
};

class UnitTree {
 public:
  UnitTree();
  UnitTree(void* ownerData);
  UnitTree(INodeProp* ownerRoot);
  ~UnitTree();

  INodeProp* GetOwner() { return _OwnerNode; };
  INodeProp* Find(void* data);
  void Delete(void* target);
  INodeProp* ToProp(void* child);
  INodeProp* GetParent(void* child);
  std::vector<INodeProp*> GetChildren(void* parent);

 private:
  UnitTree(UnitTree&) = delete;
  UnitTree& operator=(UnitTree&) = delete;

 private:
  bool isExternal = false;
  INodeProp* _OwnerNode{};
};

#endif  // !_DEPEND_NODE_H_
