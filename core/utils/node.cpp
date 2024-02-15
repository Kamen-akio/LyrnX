#include "./node.h"

static INodeProp* _Find(INodeProp* owner, void* target) {
  if (!owner)
    return owner;

  if (owner->ptrUnit == target)
    return owner;

  INodeProp *bro{}, *child{};
  if (owner->broUnit) {
    bro = _Find(owner->broUnit, target);
  }

  if (owner->childUnit) {
    child = _Find(owner->childUnit, target);
  }

  if (bro && child)
    throw new std::exception(
        "Please do not make one unit to the child of different parent units. ");

  if (bro)
    return bro;

  return child;
}

INodeProp* UnitTree::Find(void* target) {
  return _Find(_OwnerNode, target);
}

std::vector<INodeProp*> UnitTree::GetChildren(void* parent) {
  std::vector<INodeProp*> ret;
  INodeProp* child{};
  auto target = Find(parent);

  if (!target->childUnit)
    return ret;

  child = target->childUnit;
  while (child != nullptr) {
    ret.push_back(child);
    child = child->broUnit;
  }

  return ret;
}

INodeProp* UnitTree::ToProp(void* child) {
  return _Find(_OwnerNode, child);
}

static INodeProp* _FindParent(INodeProp* owner, void* target) {
  if (owner->childUnit->ptrUnit == target)
    return owner->childUnit;

  auto searchTarget = owner;
  while (searchTarget != nullptr) {
    INodeProp *bro{}, *child{};
    if (searchTarget->broUnit)
      bro = _FindParent(searchTarget->broUnit, target);

    if (searchTarget->childUnit)
      child = _FindParent(searchTarget->childUnit, target);

    if (bro && child)
      throw new std::exception(
          "Please do not make one unit to the child of different parent "
          "units. ");

    if (bro)
      return bro;

    return child;
  }

  return owner;
}

INodeProp* UnitTree::GetParent(void* child) {
  return _FindParent(_OwnerNode, child);
}

static void _Destory(INodeProp* owner) {
  if (owner->broUnit)
    _Destory(owner->broUnit);

  if (owner->childUnit)
    _Destory(owner->childUnit);

  delete owner->ptrUnit;
  delete owner;
}

static void _Delete(INodeProp* owner, void* target) {
  if (owner->ptrUnit == target)
    return _Destory(owner);

  if (owner->broUnit == target) {
    if (owner->broUnit->broUnit) {
      auto deleteUnit = owner->broUnit;
      owner->broUnit = owner->broUnit->broUnit;

      return _Destory(deleteUnit);
    }
  }

  if (owner->childUnit == target) {
    if (owner->childUnit->broUnit) {
      auto deleteUnit = owner->childUnit;
      owner->childUnit = owner->childUnit->broUnit;

      return _Destory(deleteUnit);
    }
  }

  if (owner->broUnit)
    _Delete(owner->broUnit, target);

  if (owner->childUnit)
    _Delete(owner->childUnit, target);
}

void UnitTree::Delete(void* target) {
  _Delete(_OwnerNode, target);
}

UnitTree::UnitTree() {
  _OwnerNode = new INodeProp();
}

UnitTree::UnitTree(INodeProp* ownerRoot) {
  _OwnerNode = ownerRoot;
  isExternal = true;
}

UnitTree::UnitTree(void* ownerData) {
  _OwnerNode = new INodeProp();
  _OwnerNode->ptrUnit = ownerData;
}

UnitTree::~UnitTree() {
  if (!isExternal)
    this->Delete(_OwnerNode);
}