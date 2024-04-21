#pragma once
#include <vector>

#ifndef _IMPL_MULTIWAYTREE_H_
#define _IMPL_MULTIWAYTREE_H_

namespace Core {
typedef void __stdcall nodeResultCallback(void* dataPtr);

struct Node {
 public:
  Node(Node* child = nullptr, Node* next = nullptr, void* ptr = nullptr)
      : _Child(child), _Next(next), _Ptr(ptr){};
  Node* _Child;
  Node* _Next;
  void* _Ptr;

  void* _this() const { return _Ptr; }

  void InsertChildBack(Node* Child) {
    if (_Child != nullptr) {
      return _Child->InsertBrotherBack(Child);
    }

    _Child = Child;
  };

  void InsertChildFront(Node* Child) {
    if (_Child != nullptr) {
      return _Child->InsertBrotherFront(Child);
    }

    _Child = Child;
  };

  void InsertBrotherBack(Node* Bro) {
    auto brother = this;
    while (brother->_Next != nullptr) {
      brother = brother->_Next;
    }
    brother->_Next = Bro;
  }

  void InsertBrotherFront(Node* Bro) {
    auto brother = GetBrotherPtr();
    _Next = Bro;
    _Next->_Next = brother;
  }

  Node* GetChildPtrFirst() const { return _Child; };
  Node* GetBrotherPtr() const { return _Next; };
};

class multiwayTree {
 public:
  multiwayTree(void* defaultRootData = nullptr,
               nodeResultCallback onDestroyEvent = nullptr);
  ~multiwayTree();

  Node* GetRootNode() const { return m_rootNode; };

  Node* FindDataPtr(void* data);
  Node* FindParent(Node* target);
  void DeleteNode(Node* target);
  void DeleteNodeEx(Node* target, nodeResultCallback callback);

 private:
  Node* m_rootNode;
  nodeResultCallback* m_onDestroyEvent;
};
}  // namespace Core

#endif