#ifndef FINDNEARTRIANGLE_H
#define FINDNEARTRIANGLE_H

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "Triangle.h"
#include "Segment.h"

namespace YSB
{
    template <class T>
    struct FindNearTriangle
    {
        //   std::vector<Triangle<T, 3>> triangleA, triangleB;

        std::pair<int, int> operator()(const Triangle<T, 3> &Tri, const Segment<T, 3> &edge,
                                       const std::vector<Triangle<T, 3>> &triangleA,
                                       const std::vector<Triangle<T, 3>> &triangleB, Real tol = TOL) const;
    };

    template <class T>
    inline std::pair<int, int> FindNearTriangle<T>::operator()(const Triangle<T, 3> &Tri, const Segment<T, 3> &edge,
                                                               const std::vector<Triangle<T, 3>> &triangleA,
                                                               const std::vector<Triangle<T, 3>> &triangleB, Real tol) const
    {
        auto normtri = normalize(cross(Tri.vert(1) - Tri.vert(0), Tri.vert(2) - Tri.vert(1))),
             edgeVec = edge[1] - edge[0];

        Real bestangle = 2 * M_PI;
        std::pair<int, int> nearTri = {-1, -1};

        auto neighbor = edge.neighborhood();
        for (auto &&idNeighTri : neighbor)
        {
            Triangle<T, 3> neighborTri;
            if (idNeighTri.first == 1)
            {
                if (triangleA.empty())
                    continue;
                neighborTri = triangleA[idNeighTri.second];
            }
            else if (idNeighTri.first == 2)
            {
                if (triangleB.empty())
                    continue;
                neighborTri = triangleB[idNeighTri.second];
            }

            Segment<T, 3> neighEdge = neighborTri.ed(neighborTri.edgeVec(edge));

            PointCompare cmp(tol);
            if (cmp.compare(edge[0], neighEdge[1]) == 0)
            {
                auto normneightri = normalize(cross(neighborTri.vert(1) - neighborTri.vert(0),
                                                    neighborTri.vert(2) - neighborTri.vert(1)));

                Real angle = atan2(norm(cross(normtri, normneightri)),
                                   dot(normtri, normneightri));
                if (dot(cross(normtri, normneightri), edgeVec) < 0)
                    angle = 2 * M_PI - angle;

                if (angle < M_PI)
                    angle = M_PI - angle;
                else
                    angle = 3 * M_PI - angle;

                if (angle < bestangle)
                {
                    bestangle = angle;
                    nearTri = idNeighTri;
                }
            }
        }
        return nearTri;
    }

} // namespace YSB

#endif // !FINDNEARTRIANGLE_H