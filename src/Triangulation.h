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
    struct itCmp : public std::less<T>
    {
        SegmentCompare cmp;
        explicit itCmp(Real tol = TOL) : cmp(tol) {}
        bool operator()(
            const T it1, const T it2) const
        {
            return cmp(*it1, *it2);
        }
    };
    // int iterationA = 0, iterationB = 0;
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
                     std::set<Segment<T, 3>, SegmentCompare>,
                     PointCompare>
                near(pCmp);
            std::set<std::pair<int, int>> done;

            for (auto &&itRs : resultA)
            {
                int k = itRs.first;
                //iterationA++;
                int idYinset = 1;
                int mDim = inputA[itRs.first].majorDim();
                auto normVec = inputA[itRs.first].normVec();
                std::pair<int, int> idInput(idYinset, itRs.first);
                done.insert(idInput);

                mapallP[idInput] = std::set<Point<T, 3>, PointCompare>(pCmp);
                mapallSeg[idInput] = std::set<Segment<T, 3>, SegmentCompare>(segCmp);
                std::set<Point<T, 3>, PointCompare> &allP = mapallP[idInput];
                std::set<Segment<T, 3>, SegmentCompare> &allSeg = mapallSeg[idInput];
                near.clear();
                // if(itRs.first == 104)
                // {
                //     int a = 1;
                // }
                this->clipSegment(itRs.second, allP, allSeg, near, pCmp, segCmp, tol);
                this->addSegment(idInput, itRs.second.second, allP, allSeg, near, segCmp, mDim, tol);
                this->generatorTriangle(inputA[itRs.first], idInput, allSeg, near, normVec,
                                        vecTriA, TriangulateA, pCmp, segCmp, tol);
                this->addSegmentToOverlap(itRs.second.second, done, allSeg,
                                          inputA, inputB, resultA, resultB, tol);
            }

            for (auto &&itRs : resultB)
            {
                //iterationB++;
                int idYinset = 2;
                int mDim = inputB[itRs.first].majorDim();
                auto normVec = inputB[itRs.first].normVec();
                std::pair<int, int> idInput(idYinset, itRs.first);
                done.insert(idInput);

                mapallP[idInput] = std::set<Point<T, 3>, PointCompare>(pCmp);
                mapallSeg[idInput] = std::set<Segment<T, 3>, SegmentCompare>(segCmp);
                std::set<Point<T, 3>, PointCompare> &allP = mapallP[idInput];
                std::set<Segment<T, 3>, SegmentCompare> &allSeg = mapallSeg[idInput];
                near.clear();
                // if(iterationB == 75)
                // {
                //     int b = iterationB;
                // }
                this->clipSegment(itRs.second, allP, allSeg, near, pCmp, segCmp, tol);
                this->addSegment(idInput, itRs.second.second, allP, allSeg, near, segCmp, mDim, tol);
                this->generatorTriangle(inputB[itRs.first], idInput, allSeg, near, normVec,
                                        vecTriB, TriangulateB, pCmp, segCmp, tol);
                this->addSegmentToOverlap(itRs.second.second, done, allSeg,
                                          inputA, inputB, resultA, resultB, tol);
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
        }

        void addSegmentToOverlap(
            const std::vector<std::pair<int, int>> &allOverlap,
            const std::set<std::pair<int, int>> &done,
            const std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            const std::vector<Triangle<T, 3>> &inputA,
            const std::vector<Triangle<T, 3>> &inputB,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultA,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> &resultB,
            Real tol = TOL)
        {
            for (auto &&idOverlapInput : allOverlap)
            {
                if (done.find(idOverlapInput) == done.end())
                {
                    if (idOverlapInput.first == 1)
                    {
                        auto it = resultA.find(idOverlapInput.second);
                        assert(it != resultA.end() && "Triangulation::dealOverlap.");
                        this->dealOverlap(inputA.at(idOverlapInput.second), allSeg, it, tol);
                    }
                    else if (idOverlapInput.first == 2)
                    {
                        auto it = resultB.find(idOverlapInput.second);
                        assert(it != resultB.end() && "Triangulation::dealOverlap.");
                        this->dealOverlap(inputB.at(idOverlapInput.second), allSeg, it, tol);
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
            Real tol = TOL)
        {
            for (auto &&seg : allSeg)
            {
                if (tri.locate(seg[0]) != Triangle<T, 3>::locType::Outer &&
                    tri.locate(seg[1]) != Triangle<T, 3>::locType::Outer)
                    ((itRs->second).first).push_back(seg);
            }
        }

        void generatorTriangle(
            const Triangle<T, 3> &Tri,
            const std::pair<int, int> &idInput,
            const std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            const std::map<Point<T, 3>,
                           std::set<Segment<T, 3>, SegmentCompare>, PointCompare> &near,
            const Vec<T, 3> normVec,
            std::vector<Triangle<T, 3>> &vecTri,
            std::map<int, std::vector<int>> &Triangulate,
            const PointCompare &pCmp,
            const SegmentCompare &segCmp,
            Real tol = TOL)
        {
            // mark every segment need be used for construct new triangle,
            // on Edge will be used 1 time, or 2 times.
            std::map<Segment<T, 3>, std::pair<int, int>, SegmentCompare> markEdge(segCmp);
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
                else if ((itSeg0->second).second == 1)
                {
                    p0 = (itSeg0->first)[1];
                    p1 = (itSeg0->first)[0];
                    (itSeg0->second).second -= 1;
                }
                else
                {
                    assert(itSeg0->second != zero && "markedge wrong.");
                }

                Real bestangle = 2 * M_PI;
                for (auto &&NextSeg : near.at(p1))
                {
                    SegmentCompare segcmp;
                    if (segcmp.compare(NextSeg, itSeg0->first) == 0)
                        continue;
                    auto np = (pCmp.compare((NextSeg)[0], p1) == 0) ? ((NextSeg)[1]) : ((NextSeg)[0]);

                    auto v0 = p0 - p1, v1 = np - p1;

                    // if(norm(cross(v1, v0)) / norm(v1) < tol)
                    // continue;

                    Real angle = atan2(norm(cross(v1, v0)), dot(v1, v0));
                    if (dot(cross(v1, v0), normVec) < 0)
                        angle = 2 * M_PI - angle;

                    if (angle < bestangle)
                    {
                        bestangle = angle;
                        p2 = np;
                    }
                }

                auto itSeg1 = markEdge.find(Segment<T, 3>(p1, p2));
                auto itSeg2 = markEdge.find(Segment<T, 3>(p2, p0));
                if (itSeg1 == markEdge.end())
                    assert(itSeg1 != markEdge.end() && "generatorTriangle itSeg1 wrong.");
                if (itSeg2 == markEdge.end())
                    assert(itSeg2 != markEdge.end() && "generatorTriangle itSeg2 wrong.");

                Segment<T, 3> Seg0(p0, p1, itSeg0->first.neighborhood());
                Segment<T, 3> Seg1(p1, p2, itSeg1->first.neighborhood());
                Segment<T, 3> Seg2(p2, p0, itSeg2->first.neighborhood());
                Segment<T, 3> segs[3] = {Seg0, Seg1, Seg2};
                Triangle<T, 3> tri(segs);
                tri.id() = vecTri.size();
                tri.inF() = Tri.inF();
                Triangulate[idInput.second].push_back(vecTri.size());
                vecTri.push_back(tri);

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
                auto iSeg0 = itSeg0->first;
                auto iSeg1 = itSeg1->first;
                auto iSeg2 = itSeg2->first;

                if (markEdge[iSeg0] == zero)
                    markEdge.erase(iSeg0);
                if (markEdge[iSeg1] == zero)
                    markEdge.erase(iSeg1);
                if (markEdge[iSeg2] == zero)
                    markEdge.erase(iSeg2);
            }
        }

        void clipSegment(
            const std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>> &itRs,
            std::set<Point<T, 3>, PointCompare> &allP,
            std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            std::map<Point<T, 3>,
                     std::set<Segment<T, 3>, SegmentCompare>, PointCompare> &near,
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
                    Segment<T, 3> newSeg(seg);
                    auto it = clip.find(newSeg);
                    if (it != clip.end())
                    {
                        newSeg.combineNeighbor(it->first);
                        clip.erase(it);
                    }
                    else
                    {
                        allP.insert(seg[0]);
                        allP.insert(seg[1]);
                    }
                    clip.insert({newSeg, std::set<Point<T, 3>, PointCompare>(pCmp)});
                }
            }

            // For each segment seg, add point on seg into clip.
            for (auto &&seg : clip)
            {
                for (auto itp = allP.begin(); itp != allP.end(); ++itp)
                {
                    if ((seg.first).containPoint(*itp, seg.first.majorDim(), tol) != Segment<T, 3>::locType::Outer)
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
                    Segment<T, 3> newSeg(p0, p1, seg.first.neighborhood());
                    auto itSeg = allSeg.find(newSeg);
                    if (itSeg != allSeg.end())
                    {
                        newSeg.combineNeighbor(*itSeg);
                        allSeg.erase(itSeg);
                    }
                    auto insertinfo = allSeg.insert(newSeg);
                    // itSeg = insertinfo.first;
                    if (insertinfo.second == false)
                        assert(false && "insert error.");

                    // itCmp<typename std::set<Segment<T, 3>, SegmentCompare>::iterator> cmp(tol);
                    // std::set<typename std::set<Segment<T, 3>, SegmentCompare>::iterator,
                    //          itCmp<typename std::set<Segment<T, 3>, SegmentCompare>::iterator>>
                    //     itSet(cmp);
                    // itSet.insert(itSeg);

                    auto itNear = near.insert({p0, {newSeg}});
                    if (itNear.second == false)
                        ((itNear.first)->second).insert(newSeg);

                    itNear = near.insert({p1, {newSeg}});
                    if (itNear.second == false)
                        ((itNear.first)->second).insert(newSeg);
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
                     std::set<Segment<T, 3>, SegmentCompare>, PointCompare> &near,
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
                    for (auto &&Seg : near[*itp0])
                    {
                        if (segCmp.compare(Seg, newSeg) == 0)
                        {
                            exist = true;
                            break;
                        }
                    }

                    if (exist == true)
                        continue;

                    bool interInfo = false;
                    for (auto &&seg : allSeg)
                    {
                        auto interType = intersectSegSeg(newSeg, seg);
                        if (interType == Segment<T, 2>::intsType::One ||
                            interType == Segment<T, 2>::intsType::Overlap)
                        {
                            interInfo = true;
                            break;
                        }
                    }
                    if (interInfo == true)
                        continue;

                    auto itSeg = allSeg.insert(newSeg);
                    if (itSeg.second == false)
                        assert(false && "Segment already exist while not in near[p0].");

                    auto itNear = near.find(*itp0);
                    //  auto itNear = near.insert({*itp0, {itSeg.first}});
                    if (itNear == near.end())
                        assert(false && "new point while add segment.");
                    else
                        (itNear->second).insert(newSeg);

                    itNear = near.find(*itp1);
                    //   itNear = near.insert({*itp1, {itSeg.first}});
                    if (itNear == near.end())
                        assert(false && "new point while add segment.");
                    else
                        (itNear->second).insert(newSeg);
                }
            }
        }
    };

} // namespace YSB

#endif // !TRIANGULATION_H