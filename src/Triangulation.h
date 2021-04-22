#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "Triangle.h"
#include "PointCompare.h"
#include "SegmentCompare.h"
#include <map>
#include <set>

namespace YSB
{
    template <class T>
    struct Triangulation
    {
        std::map<int, std::vector<int>> TriangulateA, TriangulateB;
        //    std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> resultA, resultB;
        std::vector<Triangle<T, 3>> vecTriA, vecTriB;

        void operator()(
            const std::vector<Triangle<T, 3>> &inputA,
            const std::vector<Triangle<T, 3>> &inputB,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultA,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultB,
            Real tol = TOL)
        {
            this->tryEveryDiagonal(inputA, inputB, resultA, resultB, tol);
        }

        void tryEveryDiagonal(
            const std::vector<Triangle<T, 3>> &inputA,
            const std::vector<Triangle<T, 3>> &inputB,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultA,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultB,
            Real tol = TOL)
        {
            PointCompare pCmp(tol);
            SegmentCompare segCmp(tol);
            std::map<std::pair<int, int>, std::set<Point<T, 3>, PointCompare>> mapallP;
            std::map<std::pair<int, int>, std::set<Segment<T, 3>, SegmentCompare>> mapallSeg;
            std::map<Point<T, 3>,
                     std::set<typename std::set<Segment<T, 3>, SegmentCompare>::iterator>,
                     PointCompare>
                near(pCmp);
            std::set<std::pair<int, int>> done;

            for (auto &&itRs : resultA)
            {
                int idYinset = 1;
                int mDim = inputA[itRs.first].majorDim();
                auto normVec = inputA[itRs.first].normVec();
                std::pair<int, int> idInput(idYinset, itRs.first);
                done.insert(idInput);

                mapallP[idInput] = std::set<Point<T, 3>, PointCompare>(pCmp);
                mapallSeg[idInput] = std::set<Segment<T, 3>, SegmentCompare>(segCmp);
                std::set<Point<T, 3>, PointCompare> &allP = mapallP[idInput];
                std::set<Segment<T, 3>, SegmentCompare> &allSeg = mapallP[idInput];
                near.clear();

                this->clipSegment(itRs.second, allP, allSeg, near, pCmp, segCmp, tol);
                this->addSegment(idInput, itRs.second.second, allP, allSeg, near, mDim, segCmp, tol);
                this->generatorTriangle(inputA[itRs.first], idInput, allSeg, near, normVec,
                                        vecTriA, TriangulateA, pCmp, tol);
                this->addSegmentToOverlap(itRs.second.second, done, allSeg,
                                          inputA, inputB, resultA, resultB);
            }

            for (auto &&itRs : resultB)
            {
                int idYinset = 2;
                int mDim = inputB[itRs.first].majorDim();
                auto normVec = inputB[itRs.first].normVec();
                std::pair<int, int> idInput(idYinset, itRs.first);
                done.insert(idInput);

                mapallP[idInput] = std::set<Point<T, 3>, PointCompare>(pCmp);
                mapallSeg[idInput] = std::set<Segment<T, 3>, SegmentCompare>(segCmp);
                std::set<Point<T, 3>, PointCompare> &allP = mapallP[idInput];
                std::set<Segment<T, 3>, SegmentCompare> &allSeg = mapallP[idInput];
                near.clear();

                this->clipSegment(itRs.second, allP, allSeg, near, pCmp, segCmp, tol);
                this->addSegment(idInput, itRs.second.second, allP, allSeg, near, mDim, segCmp, tol);
                this->generatorTriangle(inputB[itRs.first], idInput, allSeg, near, normVec,
                                        vecTriB, TriangulateB, pCmp, tol);
                this->addSegmentToOverlap(itRs.second.second, done, allSeg,
                                          inputA, inputB, resultA, resultB);
            }

            this->updateEdgeNeighbor(tol);
        }

        void updateEdgeNeighbor(Real tol = TOL)
        {
            for (auto &&tri : vecTriA)
            {
                for (int ie = 0; ie < 3; ++ie)
                {
                    auto &edge = tri.ed(ie);
                    auto &oldNeighbor = edge.neighborhood();
                    std::vector<std::pair<int, int>> newNeighbor;
                    for (auto &&idInput : oldNeighbor)
                    {
                        if (idInput.first == 1)
                        {
                            for (auto &&idNeighborTri : TriangulateA[idInput.second])
                            {
                                auto &neighborTri = vecTriA[idNeighborTri];
                                if (neighborTri.edgeVec(edge, tol) != -1)
                                {
                                    newNeighbor.push_back({idInput.first, idNeighborTri});
                                }
                            }
                        }
                        if (idInput.first == 2)
                        {
                            for (auto &&idNeighborTri : TriangulateB[idInput.second])
                            {
                                auto &neighborTri = vecTriB[idNeighborTri];
                                if (neighborTri.edgeVec(edge, tol) != -1)
                                {
                                    newNeighbor.push_back({idInput.first, idNeighborTri});
                                }
                            }
                        }
                    }

                    tri.ed(ie).neighborhood() = newNeighbor;
                    if (newNeighbor.size() > 2)
                        tri.ed(ie).IntersectionSeg() = 1;
                }
            }

            for (auto &&tri : vecTriB)
            {
                for (int ie = 0; ie < 3; ++ie)
                {
                    auto &edge = tri.ed(ie);
                    auto &oldNeighbor = edge.neighborhood();
                    std::vector<std::pair<int, int>> newNeighbor;

                    for (auto &&idInput : oldNeighbor)
                    {
                        if (idInput.first == 1)
                        {
                            for (auto &&idNeighborTri : TriangulateA)
                            {
                                auto &neighborTri = vecTriA[idNeighborTri];
                                if (neighborTri.edgeVec(edge, tol) != -1)
                                {
                                    newNeighbor.push_back({idInput.first, idNeighborTri});
                                }
                            }
                        }
                        if (idInput.first == 2)
                        {
                            for (auto &&idNeighborTri : TriangulateB)
                            {
                                auto &neighborTri = vecTriB[idNeighborTri];
                                if (neighborTri.edgeVec(edge, tol) != -1)
                                {
                                    newNeighbor.push_back({idInput.first, idNeighborTri});
                                }
                            }
                        }
                    }

                    tri.ed(ie).neighborhood() = newNeighbor;
                    if (newNeighbor.size() > 2)
                        tri.ed(ie).IntersectionSeg() = 1;
                }
            }
        }

        void addSegmentToOverlap(
            const std::vector<std::pair<int, int>> &allOverlap,
            const std::set<std::pair<int, int>> &done,
            const std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            const std::vector<Triangle<T, 3>> &inputA,
            const std::vector<Triangle<T, 3>> &inputB,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultA,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultB)
        {
            for (auto &&idOverlapInput : allOverlap)
            {
                if (done.find(idOverlapInput) == done.end())
                {
                    if (idOverlapInput.first == 1)
                    {
                        auto it = resultA.find(idOverlapInput.second);
                        assert(it != resultA.end() && "Triangulation::dealOverlap.");
                        this->dealOverlap(inputA[idOverlapInput], allSeg, it);
                    }
                    else if (idOverlapInput.first == 2)
                    {
                        auto it = resultB.find(idOverlapInput.second);
                        assert(it != resultB.end() && "Triangulation::dealOverlap.");
                        this->dealOverlap(inputB[idOverlapInput], allSeg, it);
                    }
                }
            }
        }

        void dealOverlap(
            const Triangle<T, 3> tri,
            const std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            typename std::map<int,
                              std::pair<std::vector<Segment<T, 3>>,
                                        std::vector<std::pair<int, int>>>>::iterator &
                itRs,
            Real tol)
        {
            for (auto &&seg : allSeg)
            {
                if (tri.locate(seg[0]) != Triangle<T, 3>::locType::Outer &&
                    tri.locate(seg[1]) != Triangle<T, 3>::locType::Outer)
                    (itRs->first).push_back(seg);
            }
        }

        void generatorTriangle(
            const Triangle<T, 3> &Tri,
            const std::pair<int, int> &idInput,
            const std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            const std::map<Point<T, 3>,
                           std::set<typename std::set<Segment<T, 3>, SegmentCompare>::iterator>,
                           PointCompare> &near,
            const Vec<T, 3> normVec,
            std::vector<Triangle<T, 3>> &vecTri,
            std::map<int, std::vector<int>> &Triangulate,
            const PointCompare &pCmp,
            Real tol = TOL)
        {
            // mark every segment need be used for construct new triangle,
            // on Edge will be used 1 time, or 2 times.
            std::map<Segment<T, 3>, std::pair<int, int>> markEdge;
            for (auto &&seg : allSeg)
            {
                std::pair<int, int> markValue = {1, 1};
                for (int ie = 0; ie < 3; ++ie)
                {
                    if (Tri.ed(ie).containPoint(seg[0]) != Segment<T, 3>::locType::Outer &&
                        Tri.ed(ie).containPoint(seg[1]) != Segment<T, 3>::locType::Outer)
                    {
                        if (pCmp.compare(Tri.ed(ie)[0], Tri.ed(ie)[1]) == pCmp.compare(seg[0], seg[1]))
                            markValue.second = 0;
                        else
                            markValue.first = 0;

                        break;
                    }
                }

                markEdge.insert({seg, markValue});
            }

            // 边清空markEdge边构建新triangle.
            std::pair<int, int> zero({0, 0});
            while (!markEdge.empty())
            {
                auto itSeg0 = markEdge.begin();
                Point<T, 3> p0, p1, p2;
                if ((itSeg0->second).first == 1)
                {
                    p0 = (itSeg0->first)[0];
                    p1 = (itSeg0->first)[1];
                    (itSeg0->second).first -= 1;
                }
                else
                {
                    p0 = (itSeg0->first)[1];
                    p1 = (itSeg0->first)[0];
                    (itSeg0->second).first -= 1;
                }

                Real bestangle = 2 * M_PI;
                for (auto &&itNextSeg : near[p1])
                {
                    auto np = ((*itNextSeg)[0] == p1) ? ((*itNextSeg)[1]) : ((*itNextSeg)[0]);
                    auto v0 = p0 - p1, v1 = np - p1;

                    Real angle = atan2(norm(cross(v1, v0)), dot(v1, v0));
                    if (dot(cross(v1, v0), normVec) < 0)
                        angle = 2 * M_PI - angle;

                    if (angle < bestangle)
                    {
                        bestangle = angle;
                        p2 = np;
                    }
                }

                auto itSeg1 = markEdge.find(Segment<T, 3>(p1, p2)),
                     itSeg2 = markEdge.find(Segment<T, 3>(p2, p0));

                Segment<T, 3> segs[3] = {itSeg0->first, itSeg1->first, itSeg2->first};
                Triangle<T, 3> tri(segs);
                tri.id() = vecTri.size();
                tri.InF() = Tri.InF();
                Triangulate[idInput.second].push_back(vecTri.size());
                vecTri.push_back(tri);

                assert(itSeg1 != markEdge.end() && "generatorTriangle itSeg1 wrong.");
                assert(itSeg2 != markEdge.end() && "generatorTriangle itSeg2 wrong.");
                if (pCmp.compare(p1, (itSeg1->first)[0]) == 0)
                {
                    assert((itSeg1->second).first == 1 && "generatorTriangle itSeg1 wrong.");
                    (itSeg1->second).first -= 1;
                }
                else
                {
                    assert((itSeg1->second).second == 1 && "generatorTriangle itSeg1 wrong.");
                    (itSeg1->second).second -= 1;
                }
                if (pCmp.compare(p2, (itSeg2->first)[0]) == 0)
                {
                    assert((itSeg2->second).first == 1 && "generatorTriangle itSeg2 wrong.");
                    (itSeg2->second).first -= 1;
                }
                else
                {
                    assert((itSeg2->second).second == 1 && "generatorTriangle itSeg2 wrong.");
                    (itSeg2->second).second -= 1;
                }

                if ((itSeg0->second) == zero)
                    markEdge.erase(itSeg0);
                if ((itSeg1->second) == zero)
                    markEdge.erase(itSeg1);
                if ((itSeg2->second) == zero)
                    markEdge.erase(itSeg2);
            }
        }

        void clipSegment(
            const std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>> &itRs,
            std::set<Point<T, 3>, PointCompare> &allP,
            std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            std::map<Point<T, 3>,
                     std::set<typename std::set<Segment<T, 3>, SegmentCompare>::iterator>,
                     PointCompare> &near,
            const PointCompare &pCmp,
            const SegmentCompare &segCmp,
            Real tol = TOL)
        {
            // std::set<Point<T, 3>, PointCompare> allP(pCmp);
            std::map<Segment<T, 3>, std::set<Point<T, 3>, PointCompare>, SegmentCompare> clip(segCmp);

            // insert all point and segment.
            for (auto &&seg : itRs.first)
            {
                if (pCmp.compare(seg[0], seg[1]) == 0)
                {
                    allP.insert(seg[0]);
                }
                else
                {
                    auto it = clip.insert({seg, std::set<Point<T, 3>, PointCompare>(pCmp)});
                    if (it.second == false)
                        it.first.combineNeighbor(seg);
                    else
                    {
                        allP.insert(seg[0]);
                        allP.insert(seg[1]);
                    }
                }
            }

            // For each segment seg, add point on seg into clip.
            for (auto &&seg : clip)
            {
                for (auto itp = allP.begin(); itp != allP.end(); ++itp)
                {
                    if ((seg.first).containPoint(*itp) == Segment<T, 3>::locType::Inter)
                    {
                        seg.second.insert(*itp);
                    }
                }
            }

            // Cliping long segment into short only intersect on endPoint.
            for (auto &&seg : clip)
            {
                Point<T, 3> p0 = *(seg.second.begin()), p1;
                for (auto itp1 = (++seg.second.begin());
                     itp1 != seg.second.end();
                     ++itp1)
                {
                    p1 = *itp1;
                    auto itSeg = allSeg.insert(Segment<T, 3>(p0, p1, seg.neighborhood()));
                    if (itSeg.second == false)
                        itSeg.first.combineNeighbor(seg);

                    auto itNear = near.insert({p0, itSeg.first});
                    if (itNear.second == false)
                        ((itNear.first)->second).insert(itSeg.first);

                    itNear = near.insert({p1, itSeg.first});
                    if (itNear.second == false)
                        ((itNear.first)->second).insert(itSeg.first);
                    p0 = p1;
                }
                                                                                 }
        }

        void addSegment(
            const std::pair<int, int> &idInput,
            const std::vector<std::pair<int, int>> &allOverlap,
            const std::set<Point<T, 3>, PointCompare> &allP,
            std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            std::map<Point<T, 3>,
                     std::set<typename std::set<Segment<T, 3>, SegmentCompare>::iterator>,
                     PointCompare> &near,
            const SegmentCompare &segCmp,
            const int mDim, Real tol = TOL)
        {
            std::vector<std::pair<int, int>> newSegNeighbor(allOverlap);
            newSegNeighbor.push_back(idInput);

            for (auto itp0 = allP.begin(); itp0 != --allP.end(); ++itp0)
            {
                for (auto itp1 = std::next(itp0); itp1 != allP.end(); ++itp1)
                {

                    Segment<T, 3> newSeg(*itp0, *itp1, newSegNeighbor);
                    int exist = false;
                    for (auto &&itSeg : near[*itp0])
                    {
                        if (segCmp.compare(*itSeg, newSeg) == 0)
                        {
                            exist = true;
                            break;
                        }
                    }

                    if (exist == true)
                        break;

                    bool interInfo = false;
                    for (auto &&seg : allSeg)
                    {
                        if (intersectSegSeg(newSeg, seg) == Segment<T, 2>::intsType::One)
                        {
                            interInfo = true;
                            break;
                        }
                    }
                    if (interInfo == true)
                        break;

                    auto itSeg = allSeg.insert(newSeg);
                    if (itSeg.second == false)
                        assert(false && "Segment already exist while not in near[p0].");

                    auto itNear = near.insert({*itp0, itSeg.first});
                    if (itNear.second == false)
                        ((itNear.first)->second).insert(itSeg.first);

                    itNear = near.insert({*itp1, itSeg.first});
                    if (itNear.second == false)
                        ((itNear.first)->second).insert(itSeg.first);
                }
            }
        }
    };

} // namespace YSB

#endif // !TRIANGULATION_H