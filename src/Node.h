#ifndef NODE_H_TY
#define NODE_H_TY

#include <vector>

template <class T>
struct OctreeNode;

template <class T>
struct OctreeNode {
  T val;
  std::vector<int> tris[2];
  std::vector<OctreeNode<T>*> child;
  OctreeNode() {}
  ~OctreeNode() {
    for (auto sub : child)
      delete sub;
  }
};
#endif  // !NODE_H_TY
