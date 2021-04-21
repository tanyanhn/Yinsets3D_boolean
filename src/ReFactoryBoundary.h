#ifndef REFACTORYBOUNDARY_H
#define REFACTORYBOUNDARY_H

#include "SurfacePatch.h"
#include "RemoveOverlap.h"

namespace YSB
{
    template <class T>
    struct ReFactoryBoundary
    {
        // std::vector<SurfacePatch<T>> newvecSPA, newvecSPB;

        void operator()(
            std::vector<Triangle<T, 3>> &vecTriA,
            std::vector<Triangle<T, 3>> &vecTriB,
            std::vector<SurfacePatch<T>> &vecSPA,
            std::vector<SurfacePatch<T>> &vecSPB,
            std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &ClipFaces,
            std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &coClipFaces,
            Real tol = TOL)
        {
            this->work(vecTriA, vecTriB, vecSPA, vecSPB, ClipFaces, coClipFaces, tol);
        }

        void work(
            std::vector<Triangle<T, 3>> &vecTriA,
            std::vector<Triangle<T, 3>> &vecTriB,
            std::vector<SurfacePatch<T>> &vecSPA,
            std::vector<SurfacePatch<T>> &vecSPB,
            std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &ClipFaces,
            std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &coClipFaces,
            Real tol = TOL)
        {
            SegmentCompare segCmp(tol);
            std::vector<Triangle<T, 3>> *vecTriar[2] = {&vecTriA, &vecTriB};
            std::vector<SurfacePatch<T>> *vecSPar[2] = {&vecSPA, &vecSPB};
            //                                         *newvecSPar[2] = {&newvecSPA, &newvecSPB};
            // std::vector<Triangle<T, 3>> &vecTri = *(vecTriar[(idYinset - 1)]);
            // std::vector<SurfacePatch<T>> &vecSP = *(vecSPar[(idYinset - 1)]),
            //                              &newvecSP = *(newvecSPar[(idYinset - 1)]);
            std::set<std::pair<int, int>> All;
            // std::map<Segment<T, 3>, std::set<std::pair<int, int>>, SegmentCompare> mapSeg(segCmp);
            // std::set<typename std::map<Segment<T, 3>,
            //                            std::set<std::pair<int, int>>, SegmentCompare>::iterator>
            //     markSeg;
            std::map<std::pair<std::pair<int, int>, std::pair<int, int>>, std::pair<int, int>> comb;

            for (auto &&itCF : ClipFaces)
            {
                All.insert(itCF.first);
            }

            while (!All.empty())
            {
                std::pair<int, int> idVecGCS = *All.begin();
                All.erase(idVecGCS);
                const std::vector<std::pair<int, int>> &vecIdVecSP = ClipFaces.at(idVecGCS);

                for (auto &&idVecSP : vecIdVecSP)
                {
                    auto &boundary = (*(vecSPar[idVecSP.first - 1]))[idVecSP.second].bound();
                    for (auto &&it : boundary)
                    {
                        Segment<T, 3> &seg = it->first;
                        if (it.second.size() > 1)
                        {
                            std::vector<Triangle<T, 3>> triangles;
                            std::vector<std::pair<int, int>> Neighbor;
                            for (auto idTri : it.second)
                            {
                                triangles.push_back((*(vecTriar[idTri.first - 1]))[idTri.second]);
                                auto eit = find(seg.neighborhood().begin(), seg.neighborhood().end(),
                                                (*(vecTriar[idTri.first - 1]))[idTri.second]);
                                seg.neighborhood().erase(eit, seg.neighborhood().end());
                            }

                            // For each SP, boundary remain at most one triangle.
                            //And past other triangle by good pair. adjust neighbor.
                            int t0 = 0, t1 = 0;
                            while (triangles.size() > 1)
                            {
                                t1 = FNTOp(triangles[t0], it.first, triangles, tol);
                                if (t0 == FNTOp(triangles[t1], it.first, triangles, tol))
                                {
                                    int ie0 = triangles[t0].edgeVec(it.first),
                                        ie1 = triangles[t1].edgeVec(it.first);
                                    Neighbor.clear();
                                    Neighbor.push_back({idVecSP.first, triangles[t0].id()});
                                    Neighbor.push_back({idVecSP.first, triangles[t1].id()});
                                    (*(vecTriar[idVecSP.first - 1]))[triangles[t0].id()].ed(ie0).neighborhood() = Neighbor;
                                    (*(vecTriar[idVecSP.first - 1]))[triangles[t1].id()].ed(ie1).neighborhood() = Neighbor;
                                    (*(vecTriar[idVecSP.first - 1]))[triangles[t0].id()].ed(ie0).IntersectionSeg() = 0;
                                    (*(vecTriar[idVecSP.first - 1]))[triangles[t1].id()].ed(ie1).IntersectionSeg() = 0;
                                    triangles.erase(std::advance(triangles.begin(), t0));
                                    triangles.erase(std::advance(triangles.begin(), t1));
                                }
                                else
                                    t0 = t1;
                            }

                            // deal the last is not pasted triangle.neighbor and boundary or erase the boundary.
                            it.second.clear();
                            if (triangles.size() == 1)
                            {
                                it.second.push_back({idVecSP.first, triangles[0].id()});
                                seg.neighborhood().push_back({idVecSP.first, triangles[0].id()});
                                int ie = triangles[0].edgeVec(it.first);
                                (*(vecTriar[idVecSP.first - 1]))[triangles[0].id()].ed(ie) = seg;
                            }

                            // Get the boundary also contained by which SP and GCS.
                            // And adjust these boundary and triangle.ed.
                            auto idOtherTris = seg.neighborhood();

                            std::set<std::pair<int, int>> idOtherSPs, idOtherGCSs;
                            for (auto &&itTri : idOtherTris)
                            {
                                auto idOtherSP = (*(vecTriar[itTri.first - 1]))[itTri.second].InF();
                                idOtherSPs.insert(idOtherSP);
                                idOtherGCSs.insert(coClipFaces.at(idOtherSP)[0]);

                                // Pull otherTris along erased segment.
                                int ie = (*(vecTriar[itTri.first - 1]))[itTri.second].edgeVec(seg, tol);
                                (*(vecTriar[itTri.first - 1]))[itTri.second].ed(ie).neighborhood() = idOtherTris;
                            }

                            for (auto &&itSP : idOtherSPs)
                            {
                                auto it = (*(vecSPar[itSP.first - 1]))[itSP.second].bound().find(seg);
                                assert(it != (*(vecSPar[itSP.first - 1]))[itSP.second].bound().end());
                                auto se = it->second;
                                (*(vecSPar[itSP.first - 1]))[itSP.second].bound().erase(it);
                                (*(vecSPar[itSP.first - 1]))[itSP.second].bound().insert({seg, se});
                            }

                            // while only one GCS contain the seg. erase the intersection
                            // And past the GCS' s ClipFaces  SPs cross seg by good pair.
                            if (idOtherGCSs.size() == 1)
                            {
                                All.insert(*(idOtherGCSs.begin()));

                                it.first = seg;
                                it.second = seg.neighborhood();
                                std::vector<Triangle<T, 3>> triangles;
                                std::vector<std::pair<int, int>> Neighbor;
                                for (auto idTri : it.second)
                                {
                                    triangles.push_back((*(vecTriar[idTri.first - 1]))[idTri.second]);
                                }

                                // Past by good pair.
                                int t0 = 0, t1 = 0;
                                while (triangles.size() > 1)
                                {
                                    t1 = FNTOp(triangles[t0], it.first, triangles, tol);
                                    if (t0 == FNTOp(triangles[t1], it.first, triangles, tol))
                                    {
                                        int ie0 = triangles[t0].edgeVec(it.first),
                                            ie1 = triangles[t1].edgeVec(it.first);
                                        Neighbor.clear();
                                        Neighbor.push_back({idVecSP.first, triangles[t0].id()});
                                        Neighbor.push_back({idVecSP.first, triangles[t1].id()});
                                        (*(vecTriar[idVecSP.first - 1]))[triangles[t0].id()].ed(ie0).neighborhood() = Neighbor;
                                        (*(vecTriar[idVecSP.first - 1]))[triangles[t1].id()].ed(ie1).neighborhood() = Neighbor;
                                        (*(vecTriar[idVecSP.first - 1]))[triangles[t0].id()].ed(ie0).IntersectionSeg() = 0;
                                        (*(vecTriar[idVecSP.first - 1]))[triangles[t1].id()].ed(ie1).IntersectionSeg() = 0;

                                        // Past SP in the GCS get new SP cross seg by good pair.
                                        auto idSP = std::make_pair(triangles[t0].InF(), triangles[t1].InF());
                                        auto itcomp = comb.find(idSP);
                                        if (itcomp == comb.end())
                                        {
                                            comb.insert({idSP, make_pair(idSP.first.first, vecSPar[idSP.first.first]->size())});
                                            vecSPar[idSP.first.first].emplace_back(
                                                (*(vecSPar[idSP.first.first - 1]))[idSP.first.second],
                                                (*(vecSPar[idSP.second.first - 1]))[idSP.second.second]);
                                            (*(vecSPar[idSP.first.first - 1]))[idSP.first.second].IfRemoved() = true;
                                            (*(vecSPar[idSP.second.first - 1]))[idSP.second.second].IfRemoved() = true;

                                            coClipFaces[comb[idSP]].push_back(*(idOtherGCSs.begin()));
                                            auto eit = find(ClipFaces[*(idOtherGCSs.begin())].begin(),
                                                            ClipFaces[*(idOtherGCSs.begin())].end(), idSP.first);
                                            ClipFaces[*(idOtherGCSs.begin())].erase(eit, ClipFaces[*(idOtherGCSs.begin())].end());
                                            eit = find(ClipFaces[*(idOtherGCSs.begin())].begin(),
                                                       ClipFaces[*(idOtherGCSs.begin())].end(), idSP.second);
                                            ClipFaces[*(idOtherGCSs.begin())].erase(eit, ClipFaces[*(idOtherGCSs.begin())].end());
                                            ClipFaces[*(idOtherGCSs.begin())].push_back(comb[idSP]);
                                        }

                                        // if ((*(vecSPar[idSP.first.first - 1]))[comb[idSP]].bound()[seg].size() == 2)
                                        // {
                                        //     (*(vecSPar[idSP.first.first - 1]))[comb[idSP]].bound().erase(seg);
                                        // }
                                        // else
                                        // {
                                        //     Segment<T, 3> newseg(seg);

                                        // erase seg from new SP.boundary.
                                        (*(vecSPar[idSP.first.first - 1]))[comb[idSP]].bound().erase(seg);

                                        //     for (auto pai = remainder.begin(); pai != remainder.end(); ++pai)
                                        //     {
                                        //         if (*pai == Neighbor[0] || *pai == Neighbor[1])
                                        //         {
                                        //             remainder.erase(pai);
                                        //         }
                                        //     }
                                        //     newseg.neighborhood() = remainder;
                                        //     for (auto pai = remainder.begin(); pai != remainder.end(); ++pai)
                                        //     {
                                        //         int ie = (*(vecTriar[pai->first - 1]))[pai->second].edgeVec(seg);
                                        //         (*(vecTriar[pai->first - 1]))[pai->second].ed(ie) = newseg;
                                        //     }
                                        // }

                                        triangles.erase(std::advance(triangles.begin(), t0));
                                        triangles.erase(std::advance(triangles.begin(), t1));
                                    }
                                    else
                                        t0 = t1;
                                }
                            }
                        }
                    }

                    for (auto it = boundary.begin(); it != boundary.end(); ++it)
                    {
                        if ((it->second).size() == 0)
                            boundary.erase(it);
                    }
                }
            }
        }
    };

    // template <class T>
    // struct ReduceIntersection
    // {
    //     std::vector<SurfacePatch<T>> newvecSPA, newvecSPB;

    //     void operator()(
    //         std::vector<Triangle<T, 3>> &vecTriA,
    //         std::vector<Triangle<T, 3>> &vecTriB,
    //         const std::vector<SurfacePatch<T>> &vecSPA,
    //         const std::vector<SurfacePatch<T>> &vecSPB,
    //         const std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &ClipFaces,
    //         const std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &coClipFaces,
    //         Real tol = TOL)
    //     {
    //         std::vector<Triangle<T, 3>> *vecTriar[2] = {&vecTriA, &vecTriB};
    //         std::vector<SurfacePatch<T>> *vecSPar[2] = {&vecSPA, &vecSPB},
    //                                      *newvecSPar[2] = {&newvecSPA, &newvecSPB};
    //         this->operator()(vecTriar, vecSPar, newvecSPar, ClipFaces, coClipFaces, 1, tol);
    //         this->operator()(vecTriar, vecSPar, newvecSPar, ClipFaces, coClipFaces, 2, tol);
    //     }

    //     void operator()(
    //         std::vector<Triangle<T, 3>> *vecTriar,
    //         std::vector<SurfacePatch<T>> *vecSPar,
    //         std::vector<SurfacePatch<T>> *newvecSPar,
    //         std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &ClipFaces,
    //         std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> &coClipFaces,
    //         const int idYinset, Real tol = TOL)
    //     {
    //         SegmentCompare segCmp(tol);
    //         // std::vector<Triangle<T, 3>> &vecTri = *(vecTriar[(idYinset - 1)]);
    //         std::vector<SurfacePatch<T>> &vecSP = *(vecSPar[(idYinset - 1)]),
    //                                      &newvecSP = *(newvecSPar[(idYinset - 1)]);
    //         std::set<std::pair<int, int>> All;
    //         std::map<Segment<T, 3>, std::set<std::pair<int, int>>, SegmentCompare> mapSeg(segCmp);
    //         std::set<typename std::map<Segment<T, 3>,
    //                                    std::set<std::pair<int, int>>, SegmentCompare>::iterator>
    //             markSeg;

    //         for (auto &&itCF : ClipFaces)
    //         {
    //             All.insert(itCF.first);
    //         }

    //         while (!All.empty())
    //         {
    //             std::pair<int, int> idVecGCS = *All.begin();
    //             All.erase(idVecGCS);
    //             const std::vector<std::pair<int, int>> &vecIdVecSP = ClipFaces.at(idVecGCS);

    //             for (auto &&idVecSP : vecIdVecSP)
    //             {
    //                 const auto &boundary = vecSP[idVecSP.second].bound();
    //                 for (auto &&e : boundary)
    //                 {
    //                     auto itMap = mapSeg.insert({e, std::vector<std::pair<int, int>>()});
    //                     auto itSet = ((itMap.first)->second).insert(idVecSP);

    //                     // mark that segment need delete.
    //                     if (((itMap.first)->second).size() == 1 && itMap.second == true)
    //                     {
    //                         markSeg.insert(itMap.first);
    //                     }
    //                     else if (((itMap.first)->second).size() == 1 && itMap.second == false)
    //                     {
    //                     }
    //                     else if (((itMap.first)->second).size() == 2 && itSet.second == true)
    //                     {
    //                         markSeg.erase(itMap.first);
    //                     }
    //                     else if (((itMap.first)->second).size() > 1 && itSet.second == false)
    //                     {
    //                     }
    //                 }
    //             }

    //             // erase segment
    //             for (auto &&itSeg : markSeg)
    //             {
    //                 std::vector<std::pair<int, int>> tris, otherTris;

    //                 Segment<T, 3> seg = itSeg.first;
    //                 auto idTris = vecSP[mapSeg[seg][0]].bound()[seg];
    //                 std::vector<std::pair<int, int>> idOtherTris = seg.neighborhood();
    //                 for (auto &&itTri : idTris)
    //                 {
    //                     auto eit = find(idOtherTris.begin(), idOtherTris.end(), itTri);
    //                     idOtherTris.erase(eit, idOtherTris.end());
    //                 }

    //                 // Past Tris along erased segment.
    //                 vecSP[mapSeg[seg][0]].bound().erase(seg);
    //                 for (auto &&itTri : idTris)
    //                 {
    //                     int ie = (*(vecTriar[itTri.first - 1]))[itTri.second].edgeVec(seg, tol);
    //                     (*(vecTriar[itTri.first - 1]))[itTri.second].ed(ie).neighborhood() = idTris;
    //                     (*(vecTriar[itTri.first - 1]))[itTri.second].ed(ie).IntersectionSeg() = 0;
    //                 }

    //                 //  Determine if the seg is intersectionSeg after pull.
    //                 int interSeg = 0;
    //                 Segment<T, 3> OtherSeg(seg);
    //                 OtherSeg.neighborhood() = idOtherTris;
    //                 std::set<std::pair<int, int>> idOtherSPs, idOtherGCSs;
    //                 for (auto &&itTri : idOtherTris)
    //                 {
    //                     auto idOtherSP = (*(vecTriar[itTri.first - 1]))[itTri.second].InF();
    //                     idOtherSPs.insert(idOtherSP);
    //                     idOtherGCSs.insert(coClipFaces.at(idOtherSP)[0]);
    //                 }

    //                 for (auto &&itSP : idOtherSPs)
    //                 {
    //                     auto it = (*(vecSPar[itSP.first - 1]))[itSP.second].bound().find(seg);
    //                     assert(it != (*(vecSPar[itSP.first - 1]))[itSP.second].bound().end());
    //                     auto se = it->second;
    //                     (*(vecSPar[itSP.first - 1]))[itSP.second].bound().erase(it);
    //                     if (idOtherGCSs.size() > 1)
    //                         (*(vecSPar[itSP.first - 1]))[itSP.second].bound().insert({OtherSeg, se});
    //                 }

    //                 // Remove Intersection.
    //                 if (idOtherGCSs.size() == 1)
    //                 {
    //                 }

    //                 // Pull otherTris along erased segment.
    //                 for (auto &&itTri : idOtherTris)
    //                 {
    //                     int ie = (*(vecTriar[itTri.first - 1]))[itTri.second].edgeVec(seg, tol);
    //                     (*(vecTriar[itTri.first - 1]))[itTri.second].ed(ie).neighborhood() = idOtherTris;
    //                     if (idOtherGCSs.size() == 1)
    //                         (*(vecTriar[itTri.first - 1]))[itTri.second].ed(ie).IntersectionSeg() = 0;
    //                 }
    //             }
    //         }
    //     }
    // };

} // namespace YSB

#endif // !REFACTORYBOUNDARY_H