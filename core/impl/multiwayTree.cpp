#include "./multiwayTree.h"

using Core::Node, Core::nodeResultCallback;

void _Noop(void* foo){};
Node* _FindParent(Node* root, Node* target);
void _Delete(Node* parent, nodeResultCallback callback = _Noop);
Node* _Find(Node* root, void* data);

Core::multiwayTree::multiwayTree(void* defaultRootData,
                                 nodeResultCallback onDestroyEvent) {
  m_rootNode = new Node();
  m_rootNode->_Ptr = defaultRootData;

  m_onDestroyEvent = onDestroyEvent;
  if (m_onDestroyEvent == nullptr) {
    m_onDestroyEvent = &_Noop;
  }
}

Core::multiwayTree::~multiwayTree() {
  DeleteNodeEx(m_rootNode, m_onDestroyEvent);
}

Node* Core::multiwayTree::FindDataPtr(void* data) {
  return _Find(m_rootNode, data);
}

Node* Core::multiwayTree::FindParent(Node* target) {
  return _FindParent(m_rootNode, target);
}

void Core::multiwayTree::DeleteNode(Node* target) { return _Delete(target); }

void Core::multiwayTree::DeleteNodeEx(Node* target,
                                      nodeResultCallback callback) {
  _Delete(target, callback);
}

Node* _FindParent(Node* root, Node* target) {
  auto currentNode = root;
  while (currentNode != nullptr) {
    if (currentNode == root) return nullptr;

    if (currentNode == target) return root;
    if (currentNode->_Child) {
      return _FindParent(currentNode->_Child, target);
    }
    currentNode = currentNode->_Next;
  }

  return nullptr;
}

void _Delete(Node* parent, nodeResultCallback callback) {
  if (!(parent->GetChildPtrFirst() || parent->GetBrotherPtr())) {
    callback(parent->_Ptr);
    delete parent;

    return;
  }

  auto node = parent->GetChildPtrFirst();
  if (node != nullptr) {
    _Delete(node, callback);
  }

  node = parent->GetBrotherPtr();
  if (node != nullptr) {
    _Delete(node, callback);
  }

  callback(parent->_Ptr);
  delete parent;
}

Node* _Find(Node* root, void* data) {
  auto currentNode = root;

  while (currentNode != nullptr) {
    if (currentNode->_Ptr == data) return currentNode;

    if (currentNode->_Child) {
      return _Find(currentNode->_Child, data);
    }

    currentNode = currentNode->_Next;
  }

  return nullptr;
}
