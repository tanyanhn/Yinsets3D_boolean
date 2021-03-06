#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <set>

#include "Config.h"
#include "Core/Interval.h"
#include "Core/Vec.h"
#include "Plane.h"
#include "PointCompare.h"
#include "Segment.h"
#include "SegmentCompare.h"

namespace YSB {
template <class T, int Dim>
class Triangle;

template <class T>
inline void RemoveTriangle(std::vector<Triangle<T, 3>>& vecTriA,
                           std::vector<Triangle<T, 3>>& vecTriB,
                           std::vector<Triangle<T, 3>>& vecTri,
                           const int id,
                           Real tol);

template <class T>
struct FindNearTriangle;

template <class T>
struct RemoveOverlap;

template <class T, int Dim>
class Triangle {
 public:
  // rename

  enum locType {
    Inter = 0,
    InEdge,
    InVertex,
    Outer,
  };

  enum intsType {
    Never = 0,
    Maybe = 1,
    Overlap = 2,
    IntsPoint = 3,
    IntsSeg = 4
  };

  friend struct FindNearTriangle<T>;
  friend struct RemoveOverlap<T>;
  friend void RemoveTriangle<T>(std::vector<Triangle<T, 3>>& vecTriA,
                                std::vector<Triangle<T, 3>>& vecTriB,
                                std::vector<Triangle<T, 3>>& vecTri,
                                const int id,
                                Real tol);

 private:
  Point<T, Dim> vertex[3];
  Segment<T, Dim> edge[3];
  std::pair<int, int> inFace = {-1, -1};
  mutable Plane<T>* pla;
  mutable int identity = -1;
  bool removed = false;

 public:
  // Constructor
  Triangle() : pla(nullptr) {}
  explicit Triangle(const Point<T, Dim>* vecP) : pla(nullptr) {
    vertex[0] = vecP[0];
    vertex[1] = vecP[1];
    vertex[2] = vecP[2];
    edge[0] = Segment<T, Dim>(vertex[0], vertex[1]);
    edge[1] = Segment<T, Dim>(vertex[1], vertex[2]);
    edge[2] = Segment<T, Dim>(vertex[2], vertex[0]);
  }
  explicit Triangle(const Segment<T, Dim>* vecSeg) : pla(nullptr) {
    vertex[0] = vecSeg[0][0];
    vertex[1] = vecSeg[1][0];
    vertex[2] = vecSeg[2][0];
    edge[0] = vecSeg[0];
    edge[1] = vecSeg[1];
    edge[2] = vecSeg[2];
  }
  Triangle& operator=(const Triangle<T, Dim>& rhs) {
    vertex[0] = rhs.vertex[0];
    vertex[1] = rhs.vertex[1];
    vertex[2] = rhs.vertex[2];
    edge[0] = rhs.edge[0];
    edge[1] = rhs.edge[1];
    edge[2] = rhs.edge[2];
    inFace = rhs.inFace;
    identity = rhs.identity;
    pla = nullptr;
    return *this;
  }
  Triangle(const Triangle<T, Dim>& rhs) { *this = rhs; }
  ~Triangle() {
    if (pla != nullptr)
      delete pla;
  };

  // Compare
  bool equal(const Triangle<T, Dim>& rhs, Real tol = TOL) const {
    PointCompare cmp(tol);
    if (cmp.compare(vertex[0], rhs.vertex[0]) == 0 ||
        cmp.compare(vertex[0], rhs.vertex[1]) == 0 ||
        cmp.compare(vertex[0], rhs.vertex[2]) == 0) {
      if (cmp.compare(vertex[1], rhs.vertex[0]) == 0 ||
          cmp.compare(vertex[1], rhs.vertex[1]) == 0 ||
          cmp.compare(vertex[1], rhs.vertex[2]) == 0) {
        if (cmp.compare(vertex[2], rhs.vertex[0]) == 0 ||
            cmp.compare(vertex[2], rhs.vertex[1]) == 0 ||
            cmp.compare(vertex[2], rhs.vertex[2]) == 0) {
          return true;
        }
      }
    }
    return false;
  }

  // Accessors
  Point<T, Dim>& vert(int i) { return vertex[i]; }
  const Point<T, Dim>& vert(int i) const { return vertex[i]; }

  Segment<T, Dim>& ed(int i) { return edge[i]; }
  const Segment<T, Dim>& ed(int i) const { return edge[i]; }

  int& id(int i = 0) { return identity += i; }
  const int& id(int i = 0) const { return identity += i; }

  std::pair<int, int>& inF() { return inFace; }
  const std::pair<int, int>& inF() const { return inFace; }

  bool IfRemoved() const { return removed; }

  void reverse() {
    auto temp = vertex[1];
    vertex[1] = vertex[2];
    vertex[2] = temp;
    auto neighbor0 = edge[0].neighborhood();
    auto neighbor1 = edge[1].neighborhood();
    auto neighbor2 = edge[2].neighborhood();
    edge[0] = Segment<T, Dim>(vertex[0], vertex[1], neighbor2);
    edge[1] = Segment<T, Dim>(vertex[1], vertex[2], neighbor1);
    edge[2] = Segment<T, Dim>(vertex[2], vertex[0], neighbor0);
  }

  // Update pointer pla.
  Plane<T>* new_pla() const {
    if (pla != nullptr)
      delete pla;
    pla = new Plane<T>(vertex[0],
                       cross(vertex[1] - vertex[0], vertex[2] - vertex[0]));
    pla->normVec = normalize(pla->normVec) * perimeter();
    return pla;
  }

  // Return norm vector
  auto normVec() const -> decltype(pla->normVec) {
    if (pla == nullptr)
      new_pla();
    return pla->normVec;
  }

  // Find edge direction in Triangle.
  int edgeVec(const Segment<T, Dim>& seg, Real tol = TOL) const {
    SegmentCompare cmp(tol);
    int id = -1;
    for (int i = 0; i < Dim; ++i) {
      if (cmp.compare(edge[i], seg) == 0)
        id = i;
    }
    return id;
  }
  template <class T1>
  std::pair<int, int> minmax(T1 cmp) const {
    int min, max;
    if (cmp.compare(vertex[0], vertex[1]) == 1) {
      min = 0;
      max = 1;
    } else {
      min = 1;
      max = 0;
    }
    min = (cmp.compare(vertex[min], vertex[2]) == 1) ? min : 2;
    max = (cmp.compare(vertex[max], vertex[2]) == -1) ? max : 2;

    return {min, max};
  }

  Real perimeter() const {
    return norm(vertex[0] - vertex[1]) + norm(vertex[1] - vertex[2]) +
           norm(vertex[2] - vertex[0]);
  }

  bool isParallel(const Triangle<T, Dim>& tri2, Real tol = TOL) const {
    if (pla == nullptr)
      new_pla();
    if (tri2->pla == nullptr)
      tri2->new_pla();

    auto v1 = pla->normVec, v2 = tri2->pla->normVec;
    Real dist = norm(cross(v1, v2)) / norm(v1);
    return dist < tol;
  }

  int majorDim(int k = 1) const {
    if (pla == nullptr)
      new_pla();

    return pla->majorDim(k);
  }

  template <class T1, int D>
  struct projectimpl {
    Triangle<T, D - 1> operator()(const Triangle<T1, D>& tri, int d) {
      if (d == -1) {
        d = tri.majorDim();
      }
      Point<T, Dim - 1> v[3];
      v[0] = tri.vert(0).project(d);
      v[1] = tri.vert(1).project(d);
      v[2] = tri.vert(2).project(d);
      return Triangle<T, Dim - 1>(v);
    }
  };  // namespace YSB

  template <class T1>
  struct projectimpl<T1, 2> {
    Triangle<T, 1> operator()(const Triangle<T1, 2>& tri, int d) {
      Point<T, 1> v[3];
      v[0] = tri.vert(0).project(d);
      v[1] = tri.vert(1).project(d);
      v[2] = tri.vert(2).project(d);
      return Triangle<T, 1>(v);
    }
  };

  Triangle<T, Dim - 1> project(int d = -1) const {
    projectimpl<T, Dim> impl;
    return impl(*this, d);
  }

  // Calculating barycentric coordinates
  int barycentric(const Point<T, Dim>& p, Real* co, Real tol = TOL) const {
    if (pla == nullptr)
      new_pla();
    Vec<T, Dim> A[3], E[3];
    A[0] = p - vertex[1], A[1] = p - vertex[2], A[2] = p - vertex[0];
    if (norm(dot(A[0], pla->normVec)) > tol) {
      return 0;
    }

    int m = 0;
    if (std::abs(pla->normVec[0]) < tol) {
      m = (std::abs(pla->normVec[1]) > std::abs(pla->normVec[2])) ? (1) : (2);
    }
    E[0] = vertex[2] - vertex[1];
    E[1] = vertex[0] - vertex[2];
    E[2] = vertex[1] - vertex[0];

    Vec<T, Dim> area, subarea[3];

    // for (auto i = 0; i < 3; ++i) {
    //   subarea = cross(E[i], A[i]);
    //   co[i] = (subarea / area)[m];
    // }

    for (auto i = 0; i < 3; ++i) {
      subarea[i] = cross(E[i], A[i]);
      area = area + subarea[i];
    }

    for (auto i = 0; i < 3; ++i) {
      co[i] = (subarea[i] / area)[m];
    }
    return 1;
  }

  // Get Point from its barycentric.
  Point<T, Dim> barycentric(Real* co) const {
    Vec<T, Dim> A[3];
    Point<T, Dim> zero(0.0);
    A[0] = vertex[0] - zero;
    A[1] = vertex[1] - zero;
    A[2] = vertex[2] - zero;

    return zero + A[0] * co[0] + A[1] * co[1] + A[2] * co[2];
  }

  // Locating Point with Triangle.
  Triangle<T, Dim>::locType locate(const Point<T, Dim>& p,
                                   Real tol = TOL) const {
    Real co[3];
    if (barycentric(p, co, tol) == 0)
      assert(false && "Triangle::locate(p), p not in the plane.");

    int countzero = 0;
    for (auto d = 0; d < Dim; ++d) {
      if (co[d] < -tol)
        return locType::Outer;
      if (std::abs(co[d]) < tol)
        ++countzero;
    }

    if (countzero == 0)
      return locType::Inter;
    else if (countzero == 1)
      return locType::InEdge;
    else if (countzero == 2)
      return locType::InVertex;
    else
      assert(false && "barycentric shouldn't have 3 zero.");

    return locType::Outer;
  }

  // Estimate two Triangle whether intersect.
  Triangle<T, Dim>::intsType ifIntersect(const Triangle<T, Dim>& tri2,
                                         Real tol = TOL) const {
    Real min1, min2, max1, max2;

    for (auto d = 0; d < Dim; ++d) {
      min1 = std::min(vertex[0][d], std::min(vertex[1][d], vertex[2][d]));
      max2 = std::max(tri2.vertex[0][d],
                      std::max(tri2.vertex[1][d], tri2.vertex[2][d]));
      if (min1 > max2 + tol)
        return intsType::Never;
      max1 = std::max(vertex[0][d], std::max(vertex[1][d], vertex[2][d]));
      min2 = std::min(tri2.vertex[0][d],
                      std::min(tri2.vertex[1][d], tri2.vertex[2][d]));
      if (min2 > max1 + tol)
        return intsType::Never;
    }
    if (pla == nullptr)
      new_pla();
    if (tri2.pla == nullptr)
      tri2.new_pla();

    auto v1 = pla->normVec, v2 = tri2.pla->normVec;
    Real dist = norm(cross(v1, v2)) / norm(v1);
    if (dist < tol) {  // parallel
      Vec<T, Dim> v[9] = {
          vertex[0] - tri2.vertex[0], vertex[1] - tri2.vertex[0],
          vertex[2] - tri2.vertex[0], vertex[0] - tri2.vertex[1],
          vertex[1] - tri2.vertex[1], vertex[2] - tri2.vertex[1],
          vertex[0] - tri2.vertex[2], vertex[1] - tri2.vertex[2],
          vertex[2] - tri2.vertex[2]};
      Real m1 = std::max(norm(dot(v[0], pla->normVec)),
                         std::max(norm(dot(v[1], pla->normVec)),
                                  norm(dot(v[2], pla->normVec))));
      Real m2 = std::max(norm(dot(v[3], pla->normVec)),
                         std::max(norm(dot(v[4], pla->normVec)),
                                  norm(dot(v[5], pla->normVec))));
      Real m3 = std::max(norm(dot(v[6], pla->normVec)),
                         std::max(norm(dot(v[7], pla->normVec)),
                                  norm(dot(v[8], pla->normVec))));
      Real m = m1 > m2 ? m1 : m2;
      m = m > m3 ? m : m3;
      m = m / norm(pla->normVec);
      if (m < tol) {
        return intsType::Overlap;
      } else
        return intsType::Never;
    }

    return intsType::Maybe;
  }

  // Return number of intersection.
  int intersect(Line<T, Dim>& l,
                std::vector<Point<T, Dim>>& result,
                Real tol = TOL) const {
    if (pla == nullptr)
      new_pla();

    auto dir = normalize(l.direction);
    Real dist = norm(dot(dir, pla->normVec));
    if (dist < tol) {  // parallel
      if (norm(dot(l.fixpoint - this->vertex[0], pla->normVec)) < 2 * tol) {
        int mDim = majorDim();
        l.moveFixpoint(vertex[0][l.majorDim()], l.majorDim());
        if (norm(dot(l.fixpoint - vertex[0], pla->normVec)) > tol)
          return 0;

        auto projL = l.project(mDim);
        auto projTri = this->project(mDim);
        std::vector<Point<Real, 2>> rs2D;
        projTri.intersect(projL, rs2D, tol);

        for (auto&& ip : rs2D) {
          Real co[3];
          projTri.barycentric(ip, co, tol);
          result.emplace_back(this->barycentric(co));
        }
        return rs2D.size();
      } else
        return 0;
    }

    auto p = pla->intersect(l, tol);
    if (this->locate(p, tol) != locType::Outer) {
      result.push_back(p);
      return 1;
    }
    return 0;
  }

  intsType intersect(const Triangle<T, Dim>& tri2,
                     std::vector<Segment<T, Dim>>& result,
                     Real tol = TOL) const {
    if (pla == nullptr)
      new_pla();
    if (tri2.pla == nullptr)
      tri2.new_pla();

    intsType intsT = ifIntersect(tri2, tol);

    if (intsT == intsType::Overlap) {
      int mDim = majorDim();
      auto projTri1 = this->project(mDim), projTri2 = tri2.project(mDim);

      std::vector<Segment<Real, 2>> rs2D;
      projTri1.intersect(projTri2, rs2D, tol);
      for (auto&& ip : rs2D) {
        Real stCo[3], fnCo[3];
        projTri1.barycentric(ip[0], stCo, tol);
        projTri1.barycentric(ip[1], fnCo, tol);
        result.emplace_back(
            Segment<T, Dim>(this->barycentric(stCo), this->barycentric(fnCo)));
      }
      if (result.empty())
        return intsType::Never;
      else
        return intsType::Overlap;
    } else if (intsT == Never) {
      return intsType::Never;
    } else if (intsT == Maybe) {
      PointCompare Pcmp(tol);
      int mat[2] = {-1, -1}, k = 0;
      for (auto ie1 = 0; ie1 < 3; ++ie1) {
        for (auto ie2 = 0; ie2 < 3; ++ie2) {
          if (Pcmp.compare(this->vert(ie1), tri2.vert(ie2)) == 0) {
            if (k > 1)
              assert(false && "k >1.");
            mat[k++] = ie1;
          }
        }
      }
      Line<T, Dim> l(Point<T, Dim>(0.0), Vec<T, Dim>(0.0));
      if (mat[1] != -1) {
        result.emplace_back(this->vert(mat[0]), this->vert(mat[1]));
        return intsType::IntsSeg;
      } else if (mat[0] != -1) {
        l = Line<T, Dim>(this->vert(mat[0]),
                         cross(this->pla->normVec, tri2.pla->normVec));
      } else {
        l = this->pla->intersect(*tri2.pla);
      }

      std::vector<Point<T, Dim>> pts;

      int mDim = majorDim();
      auto projL = l.project(mDim);
      auto projTri = this->project(mDim);
      std::vector<Point<Real, 2>> rs2D;
      projTri.intersect(projL, rs2D, tol);
      if (rs2D.size() == 0)
        return intsType::Never;
      for (auto&& ip : rs2D) {
        Real co[3];
        projTri.barycentric(ip, co, tol);
        pts.emplace_back(this->barycentric(co));
      }
      if (rs2D.size() == 1) {
        pts.push_back(pts[0]);
      }

      mDim = tri2.majorDim();
      projL = l.project(mDim);
      projTri = tri2.project(mDim);
      rs2D.clear();
      projTri.intersect(projL, rs2D, tol);
      if (rs2D.size() == 0)
        return intsType::Never;
      for (auto&& ip : rs2D) {
        Real co[3];
        projTri.barycentric(ip, co, tol);
        pts.emplace_back(tri2.barycentric(co));
      }
      if (rs2D.size() == 1) {
        pts.push_back(pts[2]);
      }

      std::vector<Point<T, Dim>> rsEndp;
      typename Segment<T, Dim>::intsType segintsT = solveForOverlie(
          pts[0], pts[1], pts[2], pts[3], rsEndp, tol, l.majorDim());

      if (segintsT == Segment<T, Dim>::One) {
        result.emplace_back(Segment<T, Dim>(rsEndp[0], rsEndp[0]));
        return intsType::IntsPoint;
      } else if (segintsT == Segment<T, Dim>::Overlap) {
        result.emplace_back(Segment<T, Dim>(rsEndp[0], rsEndp[1]));
        return intsType::IntsSeg;
      } else
        return intsType::Never;
    }
    return intsType::Never;
  }
};

template <>
inline int Triangle<Real, 2>::intersect(Line<Real, 2>& l,
                                        std::vector<Point<Real, 2>>& result,
                                        Real tol) const {
  for (auto i = 0; i < 3; ++i) {
    intersectSegLine(edge[i], l, result, tol);
  }
  std::set<Point<Real, 2>, PointCompare> setp;
  for (auto&& ip : result) {
    setp.insert(ip);
  }
  result.clear();
  for (auto&& ip : setp) {
    result.push_back(ip);
  }
  return result.size();
}

template <>
inline int Triangle<Real, 2>::barycentric(const Point<Real, 2>& p,
                                          Real* co,
                                          Real) const {
  Vec<Real, 2> A[3], E[3];
  A[0] = p - vertex[1], A[1] = p - vertex[2], A[2] = p - vertex[0];

  E[0] = vertex[2] - vertex[1];
  E[1] = vertex[0] - vertex[2];
  E[2] = vertex[1] - vertex[0];

  Real area = 0, subarea[3];

  for (auto i = 0; i < 3; ++i) {
    subarea[i] = cross(E[i], A[i]);
    area += subarea[i];
  }

  for (auto i = 0; i < 3; ++i) {
    co[i] = (subarea[i] / area);
  }
  return 1;
}

template <>
inline Triangle<Real, 2>::intsType Triangle<Real, 2>::intersect(
    const Triangle<Real, 2>& tri2,
    std::vector<Segment<Real, 2>>& result,
    Real tol) const {
  for (auto i = 0; i < 3; ++i) {
    Line<Real, 2> l = tri2.edge[i].getLine();

    std::vector<Point<Real, 2>> rs2D;
    this->intersect(l, rs2D, tol);
    if (rs2D.size() == 0)
      continue;
    else if (rs2D.size() == 1)
      rs2D.push_back(rs2D[0]);

    rs2D.push_back(tri2.edge[i][0]);
    rs2D.push_back(tri2.edge[i][1]);

    std::vector<Point<Real, 2>> rsEndp;
    typename Segment<Real, 2>::intsType segintsT = YSB::solveForOverlie(
        rs2D[0], rs2D[1], rs2D[2], rs2D[3], rsEndp, tol, l.majorDim());

    if (segintsT == Segment<Real, 2>::One) {
      result.emplace_back(Segment<Real, 2>(rsEndp[0], rsEndp[0]));
    } else if (segintsT == Segment<Real, 2>::Overlap) {
      result.emplace_back(Segment<Real, 2>(rsEndp[0], rsEndp[1]));
    }
  }

  for (auto i = 0; i < 3; ++i) {
    Line<Real, 2> l = this->edge[i].getLine();

    std::vector<Point<Real, 2>> rs2D;
    tri2.intersect(l, rs2D, tol);
    if (rs2D.size() == 0)
      continue;
    else if (rs2D.size() == 1)
      rs2D.push_back(rs2D[0]);

    rs2D.push_back(this->edge[i][0]);
    rs2D.push_back(this->edge[i][1]);

    std::vector<Point<Real, 2>> rsEndp;
    typename Segment<Real, 2>::intsType segintsT = YSB::solveForOverlie(
        rs2D[0], rs2D[1], rs2D[2], rs2D[3], rsEndp, tol, l.majorDim());

    if (segintsT == Segment<Real, 2>::One) {
      result.emplace_back(Segment<Real, 2>(rsEndp[0], rsEndp[0]));
    } else if (segintsT == Segment<Real, 2>::Overlap) {
      result.emplace_back(Segment<Real, 2>(rsEndp[0], rsEndp[1]));
    }
  }

  return Overlap;
}

template <class T, int Dim>
Interval<Dim> boundingBox(const Triangle<T, Dim>& c) {
  std::vector<Triangle<T, Dim>> vc{c};
  return boundingBox(vc);
}

template <class T, int Dim>
Interval<Dim> boundingBox(const std::vector<Triangle<T, Dim>>& vc) {
  Vec<Real, Dim> lower(std::numeric_limits<Real>::max());
  Vec<Real, Dim> upper = -lower;
  Point<Real, Dim> zero(0.0);
  for (const auto& c : vc) {
    for (std::size_t i = 0; i < 3; ++i) {
      lower = min(lower, c.vert(i) - zero);
      upper = max(upper, c.vert(i) - zero);
    }
  }
  return Interval<Dim>(lower, upper);
}

template <class T, int Dim>
void combine(std::vector<Triangle<T, Dim>>& tris, Real tol) {
  if (tol <= TOL)
    return;

  PointCompare PCmp(tol);
  std::vector<Point<T, 3>> all;
  for (auto&& Tri : tris) {
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
}  // namespace YSB

#endif  // !TRIANGLE_H
