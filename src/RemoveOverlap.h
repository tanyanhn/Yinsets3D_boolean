#ifndef REMOVEOVERLAP_H
#define REMOVEOVERLAP_H

#include "Triangle.h"
#include "Segment.h"
#include <algorithm>
#include <map>

namespace YSB
{
    template <class T>
    struct RemoveOverlap
    {
        //  std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA, resultB;
        //  std::vector<Triangle<T, 3>> vecTriA, vecTriB;
        //  std::vector<std::vector<int>> TriangulateA, TriangulateB;

        void operator()(std::map<int, std::vector<int>> &TriangulateA,
                        std::map<int, std::vector<int>> &TriangulateB,
                        std::vector<Triangle<T, 3>> &vecTriA,
                        std::vector<Triangle<T, 3>> &vecTriB,
                        std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultA,
                        std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultB,
                        Real tol = TOL)
        {
            this->TriangulateRemove(TriangulateA, TriangulateB, vecTriA, vecTriB, resultA, resultB, tol);
        }

        void TriangulateRemove(std::map<int, std::vector<int>> &TriangulateA,
                               std::map<int, std::vector<int>> &TriangulateB,
                               std::vector<Triangle<T, 3>> &vecTriA,
                               std::vector<Triangle<T, 3>> &vecTriB,
                               std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultA,
                               std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultB,
                               Real tol);
        // void RemoveTriangle(std::vector<Triangle<T, 3>> &vecTri, const int id);
    };

    template <class T>
    inline void RemoveOverlap<T>::TriangulateRemove(std::map<int, std::vector<int>> &TriangulateA,
                                                    std::map<int, std::vector<int>> &TriangulateB,
                                                    std::vector<Triangle<T, 3>> &vecTriA,
                                                    std::vector<Triangle<T, 3>> &vecTriB,
                                                    std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultA,
                                                    std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultB,
                                                    Real tol)
    {
        for (auto &&it : resultA)
        {
            int iA = it.first;
            int numOverlap = it.second.size();
            for (int iOverlap = 0; iOverlap < numOverlap; ++iOverlap)
            {
                auto &TriangulateAiA = TriangulateA[iA];
                int numsmalltriA = TriangulateAiA.size(), numsmalltriB;
                int iB = it.second[iOverlap].second;
                std::vector<int> tmp[2];
                if (it.second[iOverlap].first == 1)
                {
                    tmp[it.second[iOverlap].first] = TriangulateA[iB];
                }
                else
                {
                    tmp[it.second[iOverlap].first] = TriangulateB[iB];
                }
                auto &TriangulateBiB = tmp[it.second[iOverlap].first];
                numsmalltriB = TriangulateBiB.size();

                for (int ismalltriA = 0; ismalltriA < numsmalltriA; ++ismalltriA)
                {
                    for (int ismalltriB = 0; ismalltriB < numsmalltriB; ++ismalltriB)
                    {
                        if (vecTriA[TriangulateAiA[ismalltriA]].equal(vecTriA[TriangulateBiB[ismalltriB]], tol))
                        {
                            if (dot(vecTriA[TriangulateAiA[ismalltriA]].normVec(),
                                    vecTriA[TriangulateBiB[ismalltriB]].normVec()) < 0)
                            {
                                RemoveTriangle(vecTriA, vecTriB, vecTriA, TriangulateAiA[ismalltriA]);
                            }
                            RemoveTriangle(vecTriA, vecTriB, vecTriA, TriangulateBiB[ismalltriB]);
                        }
                    }
                }
            }
        }

        for (auto &&it : resultB)
        {
            int iA = it.first;
            int numOverlap = it.second.size();
            for (int iOverlap = 0; iOverlap < numOverlap; ++iOverlap)
            {
                auto &TriangulateAiA = TriangulateB[iA];
                int numsmalltriA = TriangulateAiA.size(), numsmalltriB;
                int iB = it.second[iOverlap].second;
                std::vector<int> tmp[2];
                if (it.second[iOverlap].first == 1)
                {
                    continue;
                }
                else
                {
                    tmp[it.second[iOverlap].first] = TriangulateB[iB];
                }
                auto &TriangulateBiB = tmp[it.second[iOverlap].first];
                numsmalltriB = TriangulateBiB.size();

                for (int ismalltriA = 0; ismalltriA < numsmalltriA; ++ismalltriA)
                {
                    for (int ismalltriB = 0; ismalltriB < numsmalltriB; ++ismalltriB)
                    {
                        if (vecTriA[TriangulateAiA[ismalltriA]].equal(vecTriA[TriangulateBiB[ismalltriB]], tol))
                        {
                            if (dot(vecTriA[TriangulateAiA[ismalltriA]].normVec(),
                                    vecTriA[TriangulateBiB[ismalltriB]].normVec()) < 0)
                            {
                                RemoveTriangle(vecTriA, vecTriB, vecTriA, TriangulateAiA[ismalltriA]);
                            }
                            RemoveTriangle(vecTriA, vecTriB, vecTriA, TriangulateBiB[ismalltriB]);
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
        if (vecTri[id].removed == true)
            return;
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

                    std::vector<std::pair<int, int>>::iterator eit =
                        std::remove(neighSeg.neighborhood().begin(), neighSeg.neighborhood().end(), idTri);
                    neighSeg.neighborhood().erase(eit, neighSeg.neighborhood().end());
                }
                else if (iNeighTri->first == 2)
                {
                    Triangle<T, 3> &neighTri = vecTriB[iNeighTri->second];
                    int ie = neighTri.edgeVec(edge);
                    Segment<T, 3> &neighSeg = neighTri.ed(ie);

                    std::vector<std::pair<int, int>>::iterator eit =
                        std::remove(neighSeg.neighborhood().begin(), neighSeg.neighborhood().end(), idTri);
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