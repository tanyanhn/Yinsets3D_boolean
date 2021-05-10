#ifndef PLANE_H
#define PLANE_H

#include "Line.h"

namespace YSB {
// Plane in 3D space
template <class T>
struct Plane {
  //  Norm vector and a fixpoint unique determine a plane.
  Point<T, spaceDim> fixpoint;
  Vec<T, spaceDim> normVec;

 public:
  // Constructor
  template <class T1, class T2>
  Plane(const Point<T1, spaceDim>& p, const Vec<T2, spaceDim>& d)
      : fixpoint(p), normVec(d) {}

  template <class T2>
  explicit Plane(const Plane<T2>& pl)
      : fixpoint(pl.fixpoint), normVec(pl.normVec) {}

  template <class T2>
  Plane<T>& operator=(const Plane<T2>& pl) {
    fixpoint = pl.fixpoint;
    normVec = pl.normVec;
    return *this;
  }

  ~Plane() = default;

  //  Get properly project dimension
  int majorDim(int k = 1) const { return normVec.majorDim(k); }

  // Plane intersect get Line in 3D space.
  // Must have estimate parallel.
  Line<T, 3> intersect(const Plane& pl2, Real tol = TOL) const;

  // Plane intersect Line witch isn't parallel.
  Point<T, 3> intersect(const Line<T, 3>& l, Real tol = TOL) const;
};

template <class T>
inline Point<T, 3> Plane<T>::intersect(const Line<T, 3>& l, Real tol) const {
  // Solving equations a_1 x + b_1 y + c_1 z = d_1              (1)
  // And the Line direction x / a = y / b = z / c.
  Real a = l.direction[0], b = l.direction[1], c = l.direction[2];
  Real a_1 = normVec[0], b_1 = normVec[1], c_1 = normVec[2], d_1, d;
  d_1 = a_1 * fixpoint[0] + b_1 * fixpoint[1] + c_1 * fixpoint[2];
  d = d_1 - a_1 * l.fixpoint[0] - b_1 * l.fixpoint[1] - c_1 * l.fixpoint[2];

  Real rs[3], x = 0.0, y = 0.0, z = 0.0;

  if (std::abs(a) > tol) {  // a != 0
    // Replace y = b / a * x, z = c / a * x.
    // Get k x = d_1.
    Real k = a_1 + b_1 * b / a + c_1 * c / a;
    x = d / k + l.fixpoint[0];
    y = b / a * (x - l.fixpoint[0]) + l.fixpoint[1];
    z = c / a * (x - l.fixpoint[0]) + l.fixpoint[2];
  } else if (std::abs(b) > tol) {  // b != 0
    // Replace x = a / b* y, z = c / b * y.
    // Get k y = d_1.
    Real k = a_1 * a / b + b_1 + c_1 * c / b;
    y = d / k + l.fixpoint[1];
    x = a / b * (y - l.fixpoint[1]) + l.fixpoint[0];
    z = c / b * (y - l.fixpoint[1]) + l.fixpoint[2];
  } else if (std::abs(c) > tol) {  // c != 0
    // Replace x = a / c * z, y = b / c * z.
    // Get k z = d_1.
    Real k = a_1 * a / c + b_1 * b / c + c_1;
    z = d / k + l.fixpoint[2];
    y = b / c * (z - l.fixpoint[2]) + l.fixpoint[1];
    x = a / c * (z - l.fixpoint[2]) + l.fixpoint[0];
  } else
    assert(false && "Plane parallel.");

  rs[0] = x;
  rs[1] = y;
  rs[2] = z;
  return Point<T, 3>(rs);
}

template <class T>
inline Line<T, 3> Plane<T>::intersect(const Plane& pl2, Real tol) const {
  // Cross product get line direction.
  Vec<T, 3> direction = normalize(cross(this->normVec, pl2.normVec));
  Real rs[3], x, y, z;
  // Solving equations a_1 x + b_1 y + c_1 z = d_1              (1)
  //                                        a_2 x + b_2 y + c_2 z = d_2 (2),
  // And the Line direction x / a = y / b = z / c.
  Real a = direction[0], b = direction[1], c = direction[2];

  Real a_1 = normVec[0], b_1 = normVec[1], c_1 = normVec[2], d_1,
       a_2 = pl2.normVec[0], b_2 = pl2.normVec[1], c_2 = pl2.normVec[2], d_2;
  d_1 = a_1 * fixpoint[0] + b_1 * fixpoint[1] + c_1 * fixpoint[2];
  d_2 = a_2 * pl2.fixpoint[0] + b_2 * pl2.fixpoint[1] + c_2 * pl2.fixpoint[2];

  if (std::abs(a) > std::abs(b) && std::abs(a) > std::abs(c)) {  // a != 0
    //  Let fixpoint[x] = 0
    x = 0;
    // Solving equations b_1 y + c_1 z = d_1              (3)
    //                                        b_2 y + c_2 z = d_2 (4),
    // c_2 * (1) - c_1 * (2) get k y = l

    Real co[4] = {std::abs(c_1), std::abs(c_2), std::abs(b_1), std::abs(b_2)};

    int s = 0;
    Real bi = co[0];
    for (int i = 1; i < 4; ++i) {
      if (bi < co[i]) {
        s = i;
        bi = co[i];
      }
    }

    if (s == 0) {
      Real k = c_2 * b_1 - c_1 * b_2;
      Real l = c_2 * d_1 - c_1 * d_2;

      y = l / k;
      z = (d_1 - b_1 * y) / c_1;
    } else if (s == 1) {
      Real k = c_2 * b_1 - c_1 * b_2;
      Real l = c_2 * d_1 - c_1 * d_2;

      y = l / k;
      z = (d_2 - b_2 * y) / c_2;
    } else if (s == 2) {
      Real k = c_2 * b_1 - c_1 * b_2;
      Real l = d_2 * b_1 - d_1 * b_2;

      z = l / k;
      y = (d_1 - c_1 * z) / b_1;
    } else if (s == 3) {
      Real k = c_2 * b_1 - c_1 * b_2;
      Real l = d_2 * b_1 - d_1 * b_2;

      z = l / k;
      y = (d_2 - c_2 * z) / b_2;
    } else
      assert(false && "Plane::intersect");
  } else if (std::abs(b) > std::abs(c)) {
    //  Let fixpoint[y] = 0
    y = 0;
    // Solving equations a_1 x + c_1 z = d_1              (3)
    //                                        a_2 x + c_2 z = d_2 (4),
    // c_2 * (1) - c_1 * (2) get k x = l

    Real co[4] = {std::abs(c_1), std::abs(c_2), std::abs(a_1), std::abs(a_2)};

    int s = 0;
    Real bi = co[0];
    for (int i = 1; i < 4; ++i) {
      if (bi < co[i]) {
        s = i;
        bi = co[i];
      }
    }

    if (s == 0) {
      Real k = c_2 * a_1 - c_1 * a_2;
      Real l = c_2 * d_1 - c_1 * d_2;

      x = l / k;
      z = (d_1 - a_1 * x) / c_1;
    } else if (s == 1) {
      Real k = c_2 * a_1 - c_1 * a_2;
      Real l = c_2 * d_1 - c_1 * d_2;

      x = l / k;
      z = (d_2 - a_2 * x) / c_2;
    } else if (s == 2) {
      Real k = a_2 * c_1 - a_1 * c_2;
      Real l = a_2 * d_1 - a_1 * d_2;

      z = l / k;
      x = (d_1 - c_1 * z) / a_1;
    } else if (s == 3) {
      Real k = a_2 * c_1 - a_1 * c_2;
      Real l = a_2 * d_1 - a_1 * d_2;

      z = l / k;
      x = (d_2 - c_2 * z) / a_2;
    } else
      assert(false && "Plane::intersect");
  } else if (std::abs(c) > tol) {
    //  Let fixpoint[z] = 0
    z = 0;
    // Solving equations a_1 x + b_1 y= d_1              (3)
    //                                        a_2 x + b_2 y = d_2 (4),
    // b_2 * (1) - b_1 * (2) get k x = l
    Real co[4] = {std::abs(a_1), std::abs(a_2), std::abs(b_1), std::abs(b_2)};

    int s = 0;
    Real bi = co[0];
    for (int i = 1; i < 4; ++i) {
      if (bi < co[i]) {
        s = i;
        bi = co[i];
      }
    }

    if (s == 0) {
      Real k = a_2 * b_1 - a_1 * b_2;
      Real l = a_2 * d_1 - a_1 * d_2;

      y = l / k;
      x = (d_1 - b_1 * y) / a_1;
    } else if (s == 1) {
      Real k = a_2 * b_1 - a_1 * b_2;
      Real l = a_2 * d_1 - a_1 * d_2;

      y = l / k;
      x = (d_2 - b_2 * y) / a_2;
    } else if (s == 2) {
      Real k = b_2 * a_1 - b_1 * a_2;
      Real l = b_2 * d_1 - b_1 * d_2;

      x = l / k;
      y = (d_1 - a_1 * x) / b_1;
    } else if (s == 3) {
      Real k = b_2 * a_1 - b_1 * a_2;
      Real l = b_2 * d_1 - b_1 * d_2;

      x = l / k;
      y = (d_2 - a_2 * x) / b_2;
    } else
      assert(false && "Plane::intersect");
  } else
    assert(false && "Plane::intersect, if(a,b,c)");
  rs[0] = x;
  rs[1] = y;
  rs[2] = z;
  return Line<T, 3>(Point<T, 3>(rs), direction);
}

}  // namespace YSB

#endif  // !PLANE_H
