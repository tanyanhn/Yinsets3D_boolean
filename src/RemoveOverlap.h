#ifndef REMOVEOVERLAP_H
#define REMOVEOVERLAP_H

#include "Triangle.h"
#include "Segment.h"
#include <algorithm>

namespace YSB
{
    template <class T>
    struct RemoveOverlap
    {
        //  std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA, resultB;
        //  std::vector<Triangle<T, 3>> vecTriA, vecTriB;
        //  std::vector<std::vector<int>> TriangulateA, TriangulateB;

        void operator()(std::vector<std::vector<int>> TriangulateA,
                        std::vector<std::vector<int>> TriangulateB,
                        std::vector<Triangle<T, 3>> vecTriA,
                        std::vector<Triangle<T, 3>> vecTriB,
                        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA,
                        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultB, Real tol);
        // void RemoveTriangle(std::vector<Triangle<T, 3>> &vecTri, const int id);
    };

    template <class T>
    inline void RemoveOverlap<T>::operator()(std::vector<std::vector<int>> TriangulateA,
                                             std::vector<std::vector<int>> TriangulateB,
                                             std::vector<Triangle<T, 3>> vecTriA,
                                             std::vector<Triangle<T, 3>> vecTriB,
                                             std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA,
                                             std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultB, Real tol = TOL)
    {
        int numA = resultA.size();
        for (int iA = 0; iA < numA; ++iA)
        {
            int numOverlap = resultA[iA].second.size();
            for (int iOverlap = 0; iOverlap < numOverlap; ++iOverlap)
            {
                int numsmalltriA = TriangulateA[iA].size(),
                    numsmalltriB = TriangulateB[iOverlap].size();
                for (int ismalltriA = 0; ismalltriA < numsmalltriA; ++ismalltriA)
                {
                    for (int ismalltriB = 0; ismalltriB < numsmalltriB; ++ismalltriB)
                    {
                        if (vecTriA[TriangulateA[iA][ismalltriA]].equal(vecTriB[TriangulateB[iOverlap][ismalltriB]], tol))
                        {
                            if (dot(vecTriA[TriangulateA[iA][ismalltriA]].normVec(),
                                    vecTriB[TriangulateB[iOverlap][ismalltriB]].normVec()) < 0)
                            {
                                RemoveTriangle(vecTriA, vecTriB, vecTriA, TriangulateA[iA][ismalltriA]);
                            }
                            RemoveTriangle(vecTriA, vecTriB, vecTriB, TriangulateB[iOverlap][ismalltriB]);
                        }
                    }
                }
            }
        }
    }

    template <class T>
    inline void RemoveTriangle(std::vector<Triangle<T, 3>> &vecTriA,
                               std::vector<Triangle<T, 3>> &vecTriB,
                               std::vector<Triangle<T, 3>> &vecTri, const int id)
    {
        vecTri[id].removed = true;
        int inYinset;
        if (vecTri.begin() == vecTriA.begin())
            inYinset = 1;
        else
            inYinset = 2;

        std::pair<int, int> idTri(inYinset, id);
        for (int iEdge = 0; iEdge < 3; ++iEdge)
        {
            Segment<T, 3> &edge = vecTri[id].ed(iEdge);
            std::vector<std::pair<int, int>> &neighbor = edge.neighborhood();
            for (auto iNeighTri = neighbor.begin(); iNeighTri != neighbor.end(); ++iNeighTri)
            {
                if (*iNeighTri == idTri)
                {
                    continue;
                }

                if (iNeighTri->first == 1)
                {
                    Triangle<T, 3> &neighTri = vecTriA[iNeighTri->second];
                    int ie = neighTri.edgeVec(edge);
                    Segment<T, 3> &neighSeg = neighTri.ed(ie);

                    std::vector<std::pair<int, int>>::iterator eit = std::remove(neighSeg.neighborhood().begin(), neighSeg.neighborhood().end(), idTri);
                    neighSeg.neighborhood().erase(eit, neighSeg.neighborhood().end());
                }
                else if (iNeighTri->first == 2)
                {
                    Triangle<T, 3> &neighTri = vecTriB[iNeighTri->second];
                    int ie = neighTri.edgeVec(edge);
                    Segment<T, 3> &neighSeg = neighTri.ed(ie);

                    std::vector<std::pair<int, int>>::iterator eit = std::remove(neighSeg.neighborhood().begin(), neighSeg.neighborhood().end(), idTri);
                    neighSeg.neighborhood().erase(eit, neighSeg.neighborhood().end());
                }
                else
                    assert(false && "neighbor.first != 1 || 2. in RemoveOverlap.");
            }

            edge.neighborhood().clear();
        }
    }

} // namespace YSB

#endif // !REMOVEOVERLAP_H