#ifndef CUBOID_H
#define CUBOID_H

#include <set>
#include "Config.h"
#include "Core/Interval.h"
#include "GluingCompactSurface.h"
#include "Plane.h"
#include "Point.h"
#include "PointCompare.h"
#include "Rectangle.h"
#include "Segment.h"
#include "SegmentCompare.h"
#include "Triangle.h"
#include "YinSet.h"

namespace YSB {
template <class T>
class Cuboid {
 private:
  Point<T, 3> vertex[8];
  Rectangle<T, 3> face[6];

 public:
  std::vector<Real> domain;
  Cuboid() {}
  Cuboid(const std::vector<Real>& domain) {
    // Point<T, 3> vertex[8];
    vertex[0] = Point<T, 3>{domain[0], domain[1], domain[2]};
    vertex[1] = Point<T, 3>{domain[0], domain[4], domain[2]};
    vertex[2] = Point<T, 3>{domain[0], domain[4], domain[5]};
    vertex[3] = Point<T, 3>{domain[0], domain[1], domain[5]};
    vertex[4] = Point<T, 3>{domain[3], domain[1], domain[2]};
    vertex[5] = Point<T, 3>{domain[3], domain[4], domain[2]};
    vertex[6] = Point<T, 3>{domain[3], domain[4], domain[5]};
    vertex[7] = Point<T, 3>{domain[3], domain[1], domain[5]};

    face[0] = Rectangle<T, 3>(vertex[0], vertex[3], vertex[7], vertex[4]);
    face[1] = Rectangle<T, 3>(vertex[1], vertex[2], vertex[6], vertex[5]);
    face[2] = Rectangle<T, 3>(vertex[0], vertex[1], vertex[5], vertex[4]);
    face[3] = Rectangle<T, 3>(vertex[3], vertex[2], vertex[6], vertex[7]);
    face[4] = Rectangle<T, 3>(vertex[0], vertex[1], vertex[2], vertex[3]);
    face[5] = Rectangle<T, 3>(vertex[4], vertex[5], vertex[6], vertex[7]);
    update();
  }
  Cuboid(const Point<T, 3>* vecp) {
    for (int i = 0; i < 8; i++)
      vertex[i] = vecp[i];

    face[0] = Rectangle<T, 3>(vertex[0], vertex[3], vertex[7], vertex[4]);
    face[1] = Rectangle<T, 3>(vertex[1], vertex[2], vertex[6], vertex[5]);
    face[2] = Rectangle<T, 3>(vertex[0], vertex[1], vertex[5], vertex[4]);
    face[3] = Rectangle<T, 3>(vertex[3], vertex[2], vertex[6], vertex[7]);
    face[4] = Rectangle<T, 3>(vertex[0], vertex[1], vertex[2], vertex[3]);
    face[5] = Rectangle<T, 3>(vertex[4], vertex[5], vertex[6], vertex[7]);
    update();
  }
  Cuboid(const Cuboid<T>& rhs) {
    domain.resize(6);
    for (int i = 0; i < 8; ++i)
      vertex[i] = rhs.vertex[i];
    for (int i = 0; i < 6; ++i) {
      face[i] = rhs.face[i];
      domain[i] = rhs.domain[i];
    }
  }

  void covertYin(YinSet<T>& y) {
    std::vector<Triangle<T, 3>> tris;
    tris.push_back({vertex[0], vertex[3], vertex[7]});
    tris.push_back({vertex[0], vertex[7], vertex[4]});
    tris.push_back({vertex[1], vertex[5], vertex[6]});
    tris.push_back({vertex[1], vertex[6], vertex[2]});
    tris.push_back({vertex[1], vertex[0], vertex[4]});
    tris.push_back({vertex[1], vertex[4], vertex[5]});
    tris.push_back({vertex[2], vertex[3], vertex[7]});
    tris.push_back({vertex[2], vertex[7], vertex[6]});
    tris.push_back({vertex[0], vertex[1], vertex[2]});
    tris.push_back({vertex[0], vertex[2], vertex[3]});
    tris.push_back({vertex[7], vertex[6], vertex[5]});
    tris.push_back({vertex[7], vertex[5], vertex[4]});
    GluingCompactSurface<T> gcs(tris);
    y.gcss().push_back(gcs);
  }

  Point<T, 3>& vert(int i) { return vertex[i]; }
  const Point<T, 3>& vert(int i) const { return vertex[i]; }

  Rectangle<T, 3>& ed(int i) { return face[i]; }
  const Rectangle<T, 3>& ed(int i) const { return face[i]; }

  std::vector<Cuboid<T>> divide() const;

  int intersect(const Triangle<T, 3>& tri,
                std::vector<Segment<T, 3>>& result,
                Real tol = TOL) const {
    for (int i = 0; i < 6; i++) {
      face[i].intersect(tri, result, tol);
    }
    return result.size();
  }

  // determine if a point is contained in an axis aligned cuboid
  void update() {
    T min[3], max[3];
    for (int j = 0; j < 3; j++) {
      min[j] = vertex[0][j];
      max[j] = min[j];
    }
    for (int i = 1; i < 8; i++) {
      for (int j = 0; j < 3; j++) {
        if (vertex[i][j] < min[j])
          min[j] = vertex[i][j];
        if (vertex[i][j] > max[j])
          max[j] = vertex[i][j];
      }
    }
    domain.resize(6);
    for (int i = 0; i < 3; ++i) {
      domain[i] = min[i];
      domain[i + 3] = max[i];
    }
  }

  int contain(Point<T, 3> p, Real tol = TOL) const {
    for (int j = 0; j < 3; j++) {
      if (p[j] < domain[j] - tol || p[j] > domain[j + 3] + tol)
        return false;
    }
    return true;
  }
};

// divide a cuboid in half
template <class T>
inline std::vector<Cuboid<T>> Cuboid<T>::divide() const {
  Point<T, 3> v1{(vertex[0][0] + vertex[1][0]) / 2.0,
                 (vertex[0][1] + vertex[1][1]) / 2.0,
                 (vertex[0][2] + vertex[1][2]) / 2.0};
  Point<T, 3> v2{(vertex[1][0] + vertex[2][0]) / 2.0,
                 (vertex[1][1] + vertex[2][1]) / 2.0,
                 (vertex[1][2] + vertex[2][2]) / 2.0};
  Point<T, 3> v3{(vertex[2][0] + vertex[3][0]) / 2.0,
                 (vertex[2][1] + vertex[3][1]) / 2.0,
                 (vertex[2][2] + vertex[3][2]) / 2.0};
  Point<T, 3> v4{(vertex[3][0] + vertex[0][0]) / 2.0,
                 (vertex[3][1] + vertex[0][1]) / 2.0,
                 (vertex[3][2] + vertex[0][2]) / 2.0};
  Point<T, 3> v5{(v1[0] + v3[0]) / 2.0, (v1[1] + v3[1]) / 2.0,
                 (v1[2] + v3[2]) / 2.0};

  Point<T, 3> v6{(vertex[4][0] + vertex[5][0]) / 2.0,
                 (vertex[4][1] + vertex[5][1]) / 2.0,
                 (vertex[4][2] + vertex[5][2]) / 2.0};
  Point<T, 3> v7{(vertex[5][0] + vertex[6][0]) / 2.0,
                 (vertex[5][1] + vertex[6][1]) / 2.0,
                 (vertex[5][2] + vertex[6][2]) / 2.0};
  Point<T, 3> v8{(vertex[6][0] + vertex[7][0]) / 2.0,
                 (vertex[6][1] + vertex[7][1]) / 2.0,
                 (vertex[6][2] + vertex[7][2]) / 2.0};
  Point<T, 3> v9{(vertex[7][0] + vertex[4][0]) / 2.0,
                 (vertex[7][1] + vertex[4][1]) / 2.0,
                 (vertex[7][2] + vertex[4][2]) / 2.0};
  Point<T, 3> v10{(v6[0] + v8[0]) / 2.0, (v6[1] + v8[1]) / 2.0,
                  (v6[2] + v8[2]) / 2.0};

  Point<T, 3> v11{(vertex[4][0] + vertex[0][0]) / 2.0,
                  (vertex[4][1] + vertex[0][1]) / 2.0,
                  (vertex[4][2] + vertex[0][2]) / 2.0};
  Point<T, 3> v12{(vertex[5][0] + vertex[1][0]) / 2.0,
                  (vertex[5][1] + vertex[1][1]) / 2.0,
                  (vertex[5][2] + vertex[1][2]) / 2.0};
  Point<T, 3> v13{(vertex[6][0] + vertex[2][0]) / 2.0,
                  (vertex[6][1] + vertex[2][1]) / 2.0,
                  (vertex[6][2] + vertex[2][2]) / 2.0};
  Point<T, 3> v14{(vertex[7][0] + vertex[3][0]) / 2.0,
                  (vertex[7][1] + vertex[3][1]) / 2.0,
                  (vertex[7][2] + vertex[3][2]) / 2.0};

  Point<T, 3> v15{(v11[0] + v12[0]) / 2.0, (v11[1] + v12[1]) / 2.0,
                  (v11[2] + v12[2]) / 2.0};
  Point<T, 3> v16{(v12[0] + v13[0]) / 2.0, (v12[1] + v13[1]) / 2.0,
                  (v12[2] + v13[2]) / 2.0};
  Point<T, 3> v17{(v13[0] + v14[0]) / 2.0, (v13[1] + v14[1]) / 2.0,
                  (v13[2] + v14[2]) / 2.0};
  Point<T, 3> v18{(v14[0] + v11[0]) / 2.0, (v14[1] + v11[1]) / 2.0,
                  (v14[2] + v11[2]) / 2.0};

  Point<T, 3> v19{(v15[0] + v17[0]) / 2.0, (v15[1] + v17[1]) / 2.0,
                  (v15[2] + v17[2]) / 2.0};

  Point<T, 3> vecp1[] = {vertex[0], v1, v5, v4, v11, v15, v19, v18};
  Point<T, 3> vecp2[] = {v1, vertex[1], v2, v5, v15, v12, v16, v19};
  Point<T, 3> vecp3[] = {v5, v2, vertex[2], v3, v19, v16, v13, v17};
  Point<T, 3> vecp4[] = {v4, v5, v3, vertex[3], v18, v19, v17, v14};

  Point<T, 3> vecp5[] = {v11, v15, v19, v18, vertex[4], v6, v10, v9};
  Point<T, 3> vecp6[] = {v15, v12, v16, v19, v6, vertex[5], v7, v10};
  Point<T, 3> vecp7[] = {v19, v16, v13, v17, v10, v7, vertex[6], v8};
  Point<T, 3> vecp8[] = {v18, v19, v17, v14, v9, v10, v8, vertex[7]};

  Cuboid<T> cub1(vecp1), cub2(vecp2), cub3(vecp3), cub4(vecp4);
  Cuboid<T> cub5(vecp5), cub6(vecp6), cub7(vecp7), cub8(vecp8);

  std::vector<Cuboid<T>> veccub = {cub1, cub2, cub3, cub4,
                                   cub5, cub6, cub7, cub8};
  return veccub;
}

}  // namespace YSB

#endif