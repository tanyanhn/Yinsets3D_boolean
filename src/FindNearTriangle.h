#ifndef FINDNEARTRIANGLE_H
#define FINDNEARTRIANGLE_H

#include "Segment.h"
#include "Triangle.h"
#include "TriangleCompare.h"

namespace YSB {
template <class T>
struct FindNearTriangle {
  std::pair<int, int> operator()(const Triangle<T, 3>& Tri,
                                 const Segment<T, 3>& edge,
                                 const std::vector<Triangle<T, 3>>& triangleA,
                                 const std::vector<Triangle<T, 3>>& triangleB,
                                 Real tol = TOL) const;

  int operator()(const Triangle<T, 3>& Tri,
                 const Segment<T, 3>& edge,
                 const std::vector<Triangle<T, 3>>& triangles,
                 Real tol = TOL) {
    auto e = Tri.ed(Tri.edgeVec(edge));
    auto normtri = normalize(
             cross(Tri.vert(1) - Tri.vert(0), Tri.vert(2) - Tri.vert(1))),
         edgeV = e[1] - e[0];

    Real bestangle = 2 * M_PI;
    int nearTri = -1;
    TriangleCompare tricmp(tol);

    for (size_t idNeighTri = 0; idNeighTri < triangles.size(); ++idNeighTri) {
      Triangle<T, 3> neighborTri = triangles[idNeighTri];
      if (tricmp.compare(Tri, neighborTri) == 0)
        continue;

      Segment<T, 3> neighEdge = neighborTri.ed(neighborTri.edgeVec(e, tol)),
                    Edge = e;

      PointCompare cmp(tol);
      if (cmp.compare(Edge[0], neighEdge[1]) == 0) {
        auto normneightri =
            normalize(cross(neighborTri.vert(1) - neighborTri.vert(0),
                            neighborTri.vert(2) - neighborTri.vert(1)));

        Real angle = atan2(norm(cross(normtri, normneightri)),
                           dot(normtri, normneightri));
        if (dot(cross(normtri, normneightri), edgeV) < 0)
          angle = 2 * M_PI - angle;

        if (angle < M_PI)
          angle = M_PI - angle;
        else
          angle = 3 * M_PI - angle;

        if (angle < bestangle) {
          bestangle = angle;
          nearTri = idNeighTri;
        }
      }
    }

    return nearTri;
  }
};

template <class T>
inline std::pair<int, int> FindNearTriangle<T>::operator()(
    const Triangle<T, 3>& Tri,
    const Segment<T, 3>& edge,
    const std::vector<Triangle<T, 3>>& triangleA,
    const std::vector<Triangle<T, 3>>& triangleB,
    Real tol) const {
  auto e = Tri.ed(Tri.edgeVec(edge));
  auto normtri = normalize(
           cross(Tri.vert(1) - Tri.vert(0), Tri.vert(2) - Tri.vert(1))),
       edgeV = e[1] - e[0];

  Real bestangle = 2 * M_PI;
  std::pair<int, int> nearTri = {-1, -1};
  TriangleCompare tricmp(tol);

  auto neighbor = e.neighborhood();
  for (auto&& idNeighTri : neighbor) {
    Triangle<T, 3> neighborTri;
    if (idNeighTri.first == 1) {
      if (triangleA.empty())
        continue;
      neighborTri = triangleA[idNeighTri.second];
    } else if (idNeighTri.first == 2) {
      if (triangleB.empty())
        continue;
      neighborTri = triangleB[idNeighTri.second];
    }

    if (tricmp.compare(Tri, neighborTri) == 0)
      continue;

    Segment<T, 3> neighEdge = neighborTri.ed(neighborTri.edgeVec(e, tol));

    PointCompare cmp(tol);
    if (cmp.compare(e[0], neighEdge[1]) == 0) {
      auto normneightri =
          normalize(cross(neighborTri.vert(1) - neighborTri.vert(0),
                          neighborTri.vert(2) - neighborTri.vert(1)));

      Real angle =
          atan2(norm(cross(normtri, normneightri)), dot(normtri, normneightri));
      if (dot(cross(normtri, normneightri), edgeV) < 0)
        angle = 2 * M_PI - angle;

      if (angle < M_PI)
        angle = M_PI - angle;
      else
        angle = 3 * M_PI - angle;

      if (angle < bestangle) {
        bestangle = angle;
        nearTri = idNeighTri;
      }
    }
  }
  return nearTri;
}

}  // namespace YSB

#endif  // !FINDNEARTRIANGLE_H