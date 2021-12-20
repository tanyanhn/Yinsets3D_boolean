#ifndef OCTREETRIANGLEINTERSECTION_H_TY
#define OCTREETRIANGLEINTERSECTION_H_TY

#include <algorithm>
#include <cmath>
#include <vector>
#include "Cuboid.h"
#include "Node.h"
#include "Segment.h"
#include "SegmentCompare.h"
#include "Spacedivision.h"
#include "Triangle.h"
#include "TriangleCompare.h"
#include "TriangleIntersection.h"

namespace YSB {
template <class T>
class OctreeTriangleIntersection : public TriangleIntersection<T> {
 public:
  using intsType = typename Triangle<T, 3>::intsType;
  OctreeNode<Cuboid<T>>* root;
  static SpaceDivision<T> spaceDivideOp;
  int deep;

  void initOctree(const std::vector<Triangle<T, 3>>& inputA,
                  const std::vector<Triangle<T, 3>>& inputB,
                  int depth,
                  Real tol);
  virtual long testNum(OctreeNode<Cuboid<T>>* r) {
    return (long)r->tris[0].size() * r->tris[1].size();
  }
  void calTest(OctreeNode<Cuboid<T>>* r,
               const std::vector<Triangle<T, 3>>& inputA,
               const std::vector<Triangle<T, 3>>& inputB,
               Real tol);
  void intersect(int iA,
                 int iB,
                 const std::vector<Triangle<T, 3>>& inputA,
                 const std::vector<Triangle<T, 3>>& inputB,
                 Real tol);
  int pruneTree(OctreeNode<Cuboid<T>>* r);
  void dfsConstructTree(OctreeNode<Cuboid<T>>* r,
                        int depth,
                        const std::vector<Triangle<T, 3>>& inputA,
                        const std::vector<Triangle<T, 3>>& inputB);
  std::vector<Real> coverDomain(const std::vector<Triangle<T, 3>>& inputA,
                                const std::vector<Triangle<T, 3>>& inputB,
                                Real tol);

  using TriangleIntersection<T>::resultA;
  using TriangleIntersection<T>::resultB;
  void edgeCal(const std::vector<Triangle<T, 3>>& input, Real tol, int id);
  virtual void operator()(const std::vector<Triangle<T, 3>>& inputA,
                          const std::vector<Triangle<T, 3>>& inputB,
                          Real tol = TOL,
                          int w = 1);
};

template <class T>
SpaceDivision<T> OctreeTriangleIntersection<T>::spaceDivideOp;

template <class T>
void OctreeTriangleIntersection<T>::edgeCal(
    const std::vector<Triangle<T, 3>>& input,
    Real tol,
    int id) {
  SegmentCompare cmp(tol);
  if (id == 1)
    resultA.resize(input.size());
  else
    resultB.resize(input.size());
  std::map<Segment<T, 3>, std::vector<int>, SegmentCompare> segM(cmp);
  int num = input.size();
  for (int j = 0; j < num; ++j) {
    for (int i = 0; i < 3; ++i) {
      segM[input[j].ed(i)].push_back(j);
    }
  }
  for (auto& p : segM) {
    auto seg = p.first;
    seg.neighborhood().clear();
    for (auto j : p.second) {
      seg.neighborhood().push_back(std::make_pair(id, j));
    }
    for (auto j : p.second) {
      if (id == 1) {
        resultA[j].first.push_back(seg);
      } else {
        resultB[j].first.push_back(seg);
      }
    }
  }
}

template <class T>
void OctreeTriangleIntersection<T>::intersect(
    int iA,
    int iB,
    const std::vector<Triangle<T, 3>>& inputA,
    const std::vector<Triangle<T, 3>>& inputB,
    Real tol) {
  std::vector<Segment<T, 3>> result;
  intsType type = inputA[iA].intersect(inputB[iB], result, tol);

  if (type == intsType::Never) {
    return;
  } else if (type == intsType::IntsPoint) {
  } else if (type == intsType::IntsSeg) {
  } else if (type == intsType::Overlap) {
    resultA[iA].second.push_back({2, iB});
    resultB[iB].second.push_back({1, iA});
  }

  for (auto&& iSeg : result) {
    iSeg.neighborhood().push_back(std::make_pair(1, iA));
    iSeg.neighborhood().push_back(std::make_pair(2, iB));

    resultA[iA].first.push_back(iSeg);
    resultB[iB].first.push_back(iSeg);
  }
}

template <class T>
void OctreeTriangleIntersection<T>::calTest(
    OctreeNode<Cuboid<T>>* r,
    const std::vector<Triangle<T, 3>>& inputA,
    const std::vector<Triangle<T, 3>>& inputB,
    Real tol) {
  if (r->child.empty()) {
    for (auto iA : r->tris[0]) {
      for (auto iB : r->tris[1]) {
        intersect(iA, iB, inputA, inputB, tol);
      }
    }
  } else {
    for (auto subr : r->child) {
      calTest(subr, inputA, inputB, tol);
    }
  }
}

template <class T>
int OctreeTriangleIntersection<T>::pruneTree(OctreeNode<Cuboid<T>>* r) {
  long ans = testNum(r), sub = 0;
  if (r->child.empty())
    return ans;

  for (int i = 0; i < 8; ++i) {
    sub += pruneTree(r->child[i]);
  }
  if (ans <= sub) {
    for (int i = 0; i < 8; ++i) {
      delete r->child[i];
    }
    r->child.clear();
    return ans;
  }
  return sub;
}

template <class T>
void OctreeTriangleIntersection<T>::dfsConstructTree(
    OctreeNode<Cuboid<T>>* r,
    int depth,
    const std::vector<Triangle<T, 3>>& inputA,
    const std::vector<Triangle<T, 3>>& inputB) {
  if (depth == 0 || testNum(r) == 0)
    return;

  r->child.resize(8);
  auto sub = r->val.divide();
  for (int i = 0; i < 8; ++i) {
    r->child[i] = new OctreeNode<Cuboid<T>>;
    r->child[i]->val = sub[i];
  }
  for (auto i : r->tris[0]) {
    auto ans = spaceDivideOp.intersectDividedCuboid(inputA[i], sub);
    for (int j = 0; j < 8; ++j) {
      if (ans[j] > 0) {
        r->child[j]->tris[0].push_back(i);
      }
    }
  }
  for (auto i : r->tris[1]) {
    auto ans = spaceDivideOp.intersectDividedCuboid(inputB[i], sub);
    for (int j = 0; j < 8; ++j) {
      if (ans[j] > 0) {
        r->child[j]->tris[1].push_back(i);
      }
    }
  }
  for (int j = 0; j < 8; ++j) {
    dfsConstructTree(r->child[j], depth - 1, inputA, inputB);
  }
}

template <class T>
void OctreeTriangleIntersection<T>::initOctree(
    const std::vector<Triangle<T, 3>>& inputA,
    const std::vector<Triangle<T, 3>>& inputB,
    int depth,
    Real tol) {
  auto domain = coverDomain(inputA, inputB, tol);
  root = new OctreeNode<Cuboid<T>>;
  root->val = Cuboid<T>(domain);
  for (int i = 0; i < inputA.size(); ++i) {
    root->tris[0].push_back(i);
  }
  for (int i = 0; i < inputB.size(); ++i) {
    root->tris[1].push_back(i);
  }
  dfsConstructTree(root, depth, inputA, inputB);
}

template <class T>
std::vector<Real> OctreeTriangleIntersection<T>::coverDomain(
    const std::vector<Triangle<T, 3>>& inputA,
    const std::vector<Triangle<T, 3>>& inputB,
    Real tol) {
  std::vector<Real> res1 = {inputA[0].vert(0)[0], inputA[0].vert(0)[1],
                            inputA[0].vert(0)[2], inputA[0].vert(0)[0],
                            inputA[0].vert(0)[1], inputA[0].vert(0)[2]},
                    res2 = {inputB[0].vert(0)[0], inputB[0].vert(0)[1],
                            inputB[0].vert(0)[2], inputB[0].vert(0)[0],
                            inputB[0].vert(0)[1], inputB[0].vert(0)[2]};
  Real l, u;
  for (auto& tri : inputA) {
    for (int i = 0; i < 3; ++i) {
      l = std::min({tri.vert(0)[i], tri.vert(1)[i], tri.vert(2)[i]});
      u = std::max({tri.vert(0)[i], tri.vert(1)[i], tri.vert(2)[i]});
      res1[i] = std::min(res1[i], l);
      res1[i + 3] = std::max(res1[i + 3], u);
    }
  }
  for (auto& tri : inputB) {
    for (int i = 0; i < 3; ++i) {
      l = std::min({tri.vert(0)[i], tri.vert(1)[i], tri.vert(2)[i]});
      u = std::max({tri.vert(0)[i], tri.vert(1)[i], tri.vert(2)[i]});
      res2[i] = std::min(res2[i], l);
      res2[i + 3] = std::max(res2[i + 3], u);
    }
  }
  return std::vector<Real>{std::max(res1[0], res2[0]) - 10 * tol,
                           std::max(res1[1], res2[1]) - 10 * tol,
                           std::max(res1[2], res2[2]) - 10 * tol,
                           std::min(res1[3], res2[3]) + 10 * tol,
                           std::min(res1[4], res2[4]) + 10 * tol,
                           std::min(res1[5], res2[5]) + 10 * tol};
}

template <class T>
void OctreeTriangleIntersection<T>::operator()(
    const std::vector<Triangle<T, 3>>& inputA,
    const std::vector<Triangle<T, 3>>& inputB,
    Real tol,
    int w) {
  if (inputA.empty() || inputB.empty())
    return;
  long num = inputA.size() + inputB.size();
  deep = std::log2(num) / 3;
  if (resultA.empty()) {
    edgeCal(inputA, tol, 1);
  }
  if (resultB.empty()) {
    edgeCal(inputB, tol, 2);
  }
  initOctree(inputA, inputB, deep, tol);
  pruneTree(root);
  calTest(root, inputA, inputB, tol);
  delete root;
}
}  // namespace YSB

#endif  // !OCTREETRIANGLEINTERSECTION_H_TY
