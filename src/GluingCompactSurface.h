#ifndef GLUINGCOMPACTSURFACE_H
#define GLUINGCOMPACTSURFACE_H

#include <map>
#include <vector>

#include "Config.h"
#include "SegmentCompare.h"
#include "Triangle.h"
#include "TriangleCompare.h"

namespace YSB {
enum FacType {
  Outer = 0,
  Inner = 1,
};

template <class T>
class GluingCompactSurface {
 private:
  std::vector<Triangle<T, 3>> vecTriangle;
  FacType type;

 public:
  explicit GluingCompactSurface() = default;
  explicit GluingCompactSurface(const std::vector<Triangle<T, 3>>& vt,
                                Real tol = TOL,
                                int t = -1)
      : vecTriangle(vt) {
    if (t != -1)
      type = FacType(t);
    else
      calType(tol);
  }

  GluingCompactSurface(const std::vector<std::pair<int, int>>& id,
                       const std::vector<Triangle<T, 3>>& vecTriA,
                       const std::vector<Triangle<T, 3>>& vecTriB,
                       Real tol = TOL,
                       int t = -1) {
    for (auto&& i : id) {
      if (i.first == 1)
        vecTriangle.push_back(vecTriA[i.second]);
      else if (i.first == 2)
        vecTriangle.push_back(vecTriB[i.second]);
      else
        assert(false && "GluingCompactSurface().");
    }
    if (t != -1)
      type = FacType(t);
    else
      calType(tol);
  }

  void collapse(std::vector<Triangle<T, 3>>& rs,
                int idYinset,
                int idFace) const {
    int size = rs.size();
    for (auto tri : vecTriangle) {
      tri.id() = size;
      for (auto ie = 0; ie < 3; ++ie) {
        tri.ed(ie).IntersectionSeg() = 0;
        tri.ed(ie).neighborhood().clear();
      }
      tri.inF() = {idYinset, idFace};
      rs.push_back(tri);
      ++size;
    }
  }

  FacType calType(Real tol = TOL) {
    std::random_device
        rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());  // Standard mersenne_twister_engine seeded with
                             // rd()
    std::uniform_real_distribution<> realdistrib(0, 1);
    while (1) {
      for (size_t i_tri = 0; i_tri < vecTriangle.size(); ++i_tri) {
        Triangle<T, 3> tri3D = vecTriangle[i_tri];
        int mDim = tri3D.majorDim();
        Vec<T, 3> outerVec(0.0);
        Point<T, 3> zero(0.0);
        outerVec[mDim] = GreatValue;
        Real t[3] = {
            realdistrib(gen),
            realdistrib(gen),
            realdistrib(gen),
        };
        Point<T, 3> barycenter = zero + ((tri3D.vert(0) - zero) * t[0] +
                                         (tri3D.vert(1) - zero) * t[1] +
                                         (tri3D.vert(2) - zero) * t[2]) /
                                            (t[0] + t[1] + t[2]);

        Line<T, 3> l(barycenter, outerVec);
        PointCompare cmp(tol);
        std::pair<Point<T, 3>, int> lowest = {barycenter, i_tri},
                                    highest = {barycenter, i_tri};

        for (size_t i = 0; i < vecTriangle.size(); ++i) {
          Triangle<T, 3>& tri = vecTriangle[i];
          int b = 0;
          for (int d = 0; d < 3; ++d) {
            if (d != mDim) {
              if ((tri.vert(0)[d] < barycenter[d] &&
                   tri.vert(1)[d] < barycenter[d] &&
                   tri.vert(2)[d] < barycenter[d]) ||
                  (tri.vert(0)[d] > barycenter[d] &&
                   tri.vert(1)[d] > barycenter[d] &&
                   tri.vert(2)[d] > barycenter[d]))
                b = 1;
            }
          }
          if (b == 1)
            continue;

          std::vector<Point<T, 3>> tmpRs;
          tri.intersect(l, tmpRs, tol);
          for (auto&& tmpP : tmpRs) {
            if (tmpP[mDim] < lowest.first[mDim]) {
              lowest = {tmpP, i};
            }
            if (tmpP[mDim] > highest.first[mDim]) {
              highest = {tmpP, i};
            }
          }
        }

        if (vecTriangle[lowest.second].locate(lowest.first, tol) ==
            Triangle<T, 3>::locType::Inter)
          return type = dot(outerVec, vecTriangle[lowest.second].normVec()) < 0
                            ? FacType::Inner
                            : FacType::Outer;

        if (vecTriangle[highest.second].locate(highest.first, tol) ==
            Triangle<T, 3>::locType::Inter)
          return type = dot(outerVec, vecTriangle[highest.second].normVec()) > 0
                            ? FacType::Inner
                            : FacType::Outer;
      }

      assert(false && "calType() have trouble.");
    }
  }

  std::vector<Triangle<T, 3>>& tris() { return vecTriangle; }
  const std::vector<Triangle<T, 3>>& tris() const { return vecTriangle; }

  FacType& Type() { return type; }

  const FacType& Type() const { return type; }

  void sort(int i = 1, Real tol = TOL) {
    TriangleCompare TriCmp(tol);
    std::set<Triangle<T, 3>, TriangleCompare> all(TriCmp);
    for (auto&& Tri : vecTriangle) {
      all.insert(Tri);
    }

    std::vector<Triangle<T, 3>> newvecTriangle;
    auto it = newvecTriangle.end();

    if (i > 0)
      newvecTriangle.insert(it, all.begin(), all.end());
    else
      newvecTriangle.insert(it, all.rbegin(), all.rend());

    vecTriangle.swap(newvecTriangle);
  }

  void minimize(Real tol) {
    if (tol <= TOL)
      return;

    PointCompare PCmp(tol);
    std::vector<Point<T, 3>> all;
    for (auto&& Tri : tris()) {
      for (int i = 0; i < 3; ++i) {
        int exist = 0;
        for (auto&& P : all) {
          if (PCmp.compare(P, Tri.vert(i)) == 0) {
            Tri.vert(i) = P;
            Tri.ed(i)[0] = P;
            int j = i - 1;
            if (j == -1)
              j = 2;
            Tri.ed(j)[1] = P;
            exist = 1;
          }
        }
        if (exist == 0) {
          all.push_back(Tri.vert(i));
        }
      }
    }
  }
};

}  // namespace YSB

#endif  // !GLUINGCOMPACTSURFACE_H