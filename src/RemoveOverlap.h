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
        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA, resultB;
        std::vector<Triangle<T, 3>> vecTriA, vecTriB,
            inputA, inputB;
        std::vector<std::vector<int>> TriangulateA, TriangulateB;

        void operator()();
        void RemoveTriangle(std::vector<Triangle<T, 3>> &vecTri, const int id);
    };

    template <class T>
    inline void RemoveOverlap<T>::operator()()
    {
        int numA = inputA.size();
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
                        if (vecTriA[TriangulateA[iA][ismalltriA]].equal(vecTriB[TriangulateB[iOverlap][ismalltriB]]))
                        {
                            if (dot(vecTriA[TriangulateA[iA][ismalltriA]].normVec(),
                                    vecTriB[TriangulateB[iOverlap][ismalltriB]].normVec()) < 0)
                            {
                                RemoveTriangle(vecTriA, TriangulateA[iA][ismalltriA]);
                            }
                            RemoveTriangle(vecTriB, TriangulateB[iOverlap][ismalltriB]);
                        }
                    }
                }
            }
        }
    }

    template <class T>
    inline void RemoveOverlap<T>::RemoveTriangle(std::vector<Triangle<T, 3>> &vecTri, const int id)
    {
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
                    assert(false && "neighbor.first != 1 || 2. in RemoveOverlap.")
            }

            edge.neighborhood().clear();
        }
    }

} // namespace YSB

#endif // !REMOVEOVERLAP_H