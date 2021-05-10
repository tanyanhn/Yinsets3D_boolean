#ifndef LOCATE_H
#define LOCATE_H
#include <cstdlib>
#include <vector>

#include "RemoveOverlap.h"
#include "SurfacePatch.h"
#include "Triangle.h"

namespace YSB {
template <class T>
struct Locate {
  int operator()(const std::vector<Triangle<T, 3>>& yinset,
                 const Point<T, 3>& p,
                 Real tol = TOL);
  std::vector<int> operator()(const std::vector<Triangle<T, 3>>& yinset,
                              const std::vector<Point<T, 3>>& p,
                              bool bound,
                              Real tol = TOL);
  int operator()(const std::vector<Triangle<T, 3>>& yinset,
                 std::vector<Triangle<T, 3>>& vecTri,
                 const SurfacePatch<T>& faces,
                 Real tol = TOL);
  void operator()(const std::vector<Triangle<T, 3>>& inputA,
                  const std::vector<Triangle<T, 3>>& inputB,
                  int ifBoundA,
                  int ifBoundB,
                  std::vector<Triangle<T, 3>>& vecTriA,
                  std::vector<Triangle<T, 3>>& vecTriB,
                  std::vector<SurfacePatch<T>>& vecSPA,
                  std::vector<SurfacePatch<T>>& vecSPB,
                  Real tol = TOL);
};

template <class T>
inline std::vector<int> Locate<T>::operator()(
    const std::vector<Triangle<T, 3>>& yinset,
    const std::vector<Point<T, 3>>& ps,
    bool bound,
    Real tol) {
  std::vector<int> rs;
  for (auto&& p : ps) {
    int k = this->operator()(yinset, p, tol);
    if (k == -2) {
      if (bound == true)
        k = -1;
      else
        k = 1;
    }
    rs.push_back(k);
  }
  return rs;
}

template <class T>
inline T RandomGenerate() {
  return -1.0 + 2.0 * rand() / T(RAND_MAX);
}

template <class T>
inline Vec<T, 3> RandomDirection() {
  T a[3];
  for (int i = 0; i < 3; i++)
    a[i] = RandomGenerate<T>();
  Vec<T, 3> res(a);
  return normalize(res);
}

template <class T>
inline int Locate<T>::operator()(const std::vector<Triangle<T, 3>>& yinset,
                                 const Point<T, 3>& p,
                                 Real tol) {
  // 1: in the interior of the yinset; 0: on the boundary of the yinset; -1: in
  // the exterior of the yinset
  while (1) {
    Vec<T, 3> dir = RandomDirection<T>();
    Line<T, 3> l(p, dir);
    std::vector<Point<T, 3>> intsP;
    std::vector<int> record;

    for (size_t i = 0; i < yinset.size(); i++) {
      auto tri = yinset[i];
      std::vector<Point<T, 3>> tmpres;
      if (tri.intersect(l, tmpres) == 1) {
        if (norm(p - tmpres[0]) < tol)
          return 0;

        intsP.push_back(tmpres[0]);
        record.push_back(i);
      } else if (tri.intersect(l, tmpres) == 2) {
        Segment<T, 3> s(tmpres[0], tmpres[1]);
        if (s.containPoint(p, s.majorDim()) != Segment<T, 3>::locType::Outer)
          return 0;
        else {
          intsP.push_back(tmpres[0]);
          intsP.push_back(tmpres[1]);
          record.push_back(i);
          record.push_back(i);
        }
      }
    }

    if (intsP.size() == 0)
      return -2;

    T min_D = YSB::GreatValue;
    Point<T, 3> tmpP;
    int index;
    for (size_t i = 0; i < intsP.size(); i++) {
      auto ip = intsP[i];
      if (norm(ip - p) < min_D) {
        tmpP = ip;
        min_D = norm(ip - p);
        index = i;
      }
    }

    if (yinset[record[index]].locate(tmpP) == Triangle<T, 3>::locType::Inter) {
      auto pla = yinset[record[index]].new_pla();
      Vec<T, 3> norm = pla->normVec;
      return (dot(norm, tmpP - p) > 0 ? 1 : -1);
    }
  }
}

template <class T>
inline int Locate<T>::operator()(const std::vector<Triangle<T, 3>>& yinset,
                                 std::vector<Triangle<T, 3>>& vecTri,
                                 const SurfacePatch<T>& face,
                                 Real tol) {
  const Triangle<T, 3>& tri = vecTri[face.tris()[0].second];
  Point<T, 3> zero(0.0);

  Point<T, 3> barycenter = zero + ((tri.vert(0) - zero) + (tri.vert(1) - zero) +
                                   (tri.vert(2) - zero)) /
                                      3.0;

  int rs = this->operator()(yinset, barycenter, tol);
  // assert(rs != 0 && "Locate shouldn't on Surface.");
  return rs;
}

template <class T>
inline void Locate<T>::operator()(const std::vector<Triangle<T, 3>>& inputA,
                                  const std::vector<Triangle<T, 3>>& inputB,
                                  int ifBoundA,
                                  int ifBoundB,
                                  std::vector<Triangle<T, 3>>& vecTriA,
                                  std::vector<Triangle<T, 3>>& vecTriB,
                                  std::vector<SurfacePatch<T>>& vecSPA,
                                  std::vector<SurfacePatch<T>>& vecSPB,
                                  Real tol) {
  for (auto&& iSP : vecSPA) {
    if (iSP.removed == false) {
      int k = this->operator()(inputB, vecTriA, iSP, tol);
      if (ifBoundB == 0 && k == -2)
        k = 1;
      if (k < 0) {
        iSP.removed = true;
        for (auto&& it : iSP.tris()) {
          RemoveTriangle(vecTriA, vecTriB, vecTriA, it.second, TOL);
        }
      } else if (k == 1)
        ;
      else if (k == 0)
        // assert(false && "Locate SurfacePatch have wrong.")
        ;
    }
  }

  for (auto&& iSP : vecSPB) {
    if (iSP.removed == false) {
      int k = this->operator()(inputA, vecTriB, iSP, tol);
      if (ifBoundA == 0 && k == -2)
        k = 1;
      if (k < 0) {
        iSP.removed = true;
        for (auto&& it : iSP.tris()) {
          RemoveTriangle(vecTriA, vecTriB, vecTriB, it.second, TOL);
        }
      } else if (k == 1)
        ;
      else if (k == 0)
        // assert(false && "Locate SurfacePatch have wrong.")
        ;
    }
  }
}
}  // namespace YSB

#endif