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
        //    std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA, resultB;
        std::vector<Triangle<T, 3>> vecTriA, vecTriB;

        void operator()(
            const std::vector<Triangle<T, 3>> &inputA,
            const std::vector<Triangle<T, 3>> &inputB,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> &resultA,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> &resultB,
            Real tol = TOL)
        {
            this->tryEveryDiagonal(inputA, inputB, resultA, resultB, tol);
        }

        void tryEveryDiagonal(
            const std::vector<Triangle<T, 3>> &inputA,
            const std::vector<Triangle<T, 3>> &inputB,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> &resultA,
            std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> &resultB,
            Real tol = TOL)
        {
            PointCompare pCmp(tol);
            SegmentCompare segCmp(tol);
            std::set<Point<T, 3>, PointCompare> allP(pCmp);
            std::set<Segment<T, 3>, SegmentCompare> allSeg(segCmp);
            std::map<Point<T, 3>,
                     std::set<typename std::set<Segment<T, 3>, SegmentCompare>::iterator>,
                     PointCompare>
                near(pCmp);

            for (auto &&itRs : resultA)
            {
                int idYinset = 1;
                int mDim = inputA[itRs.first].majorDim();
                std::pair<int, int> idInput(idYinset, itRs.first);
                this->clipSegment(itRs, allP, allSeg, near, pCmp, segCmp, tol);
                this->addSegment(idInput, allP, allSeg, near, mDim, segCmp, tol);
                this->generatorTriangle(inputA[itRs.first], allSeg, near, pCmp, tol);
            }
        }

        void generatorTriangle(
            const Triangle<T, 3> Tri,
            const std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            const std::map<Point<T, 3>,
                           std::set<typename std::set<Segment<T, 3>, SegmentCompare>::iterator>,
                           PointCompare> &near,
            const PointCompare &pCmp,
            Real tol = TOL)
        {
            // mark every segment need be used for construct new triangle,
            // on Edge will be used 1 time, or 2 times.
            std::map<Segment<T, 3>, std::pair<int, int>> markEdge;
            for (auto &&seg : allSeg)
            {
                bool onEdge = false;
                std::pair<int, int> markValue = {1, 1};
                for (int ie = 0; ie < 3; ++ie)
                {
                    if (Tri.ed(ie).containPoint(seg[0]) != Segment<T, 3>::locType::Outer &&
                        Tri.ed(ie).containPoint(seg[1]) != Segment<T, 3>::locType::Outer)
                    {
                        onEdge = true;
                        if (pCmp.compare(Tri.ed(ie)[0], Tri.ed(ie)[1]) == pCmp.compare(seg[0], seg[1]))
                            markValue.second = 0;
                        else
                            markValue.first = 0;

                        break;
                    }
                }

                if (onEdge)
                    markEdge.insert({seg, markValue});
                else
                    markEdge.insert({seg, markValue});

                // 边清空markEdge边构建新triangle.
                while (markEdge.empty())
                {
                    auto itSeg = markEdge.begin();
                    Point<T, 3> p0, p1, p2;
                    if ((itSeg->second).first == 1)
                    {
                        p0 = (itSeg->first)[0];
                        p1 = (itSeg->first)[1];
                    }
                    else
                    {
                        p0 = (itSeg->first)[1];
                        p1 = (itSeg->first)[0];
                    }

                    for (auto &&itNextSeg : near[p1])
                    {
                    }
                }
            }
        }

        void clipSegment(
            const std::pair<std::vector<Segment<T, 3>>, std::vector<int>> &itRs,
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
                    allP.insert(seg[0]);
                    allP.insert(seg[1]);
                    auto it = clip.insert({seg, std::set<Point<T, 3>, PointCompare>(pCmp)});
                    if (it.second == false)
                        it.first.combineNeighbor(seg);
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
                     ++itp1, p0 = p1, p1 = *itp1)
                {
                    auto itSeg = allSeg.insert(Segment<T, 3>(p0, p1, seg.neighborhood()));
                    if (itSeg.second == false)
                        itSeg.first.combineNeighbor(seg);

                    auto itNear = near.insert({p0, itSeg.first});
                    if (itNear.second == false)
                        ((itNear.first)->second).insert(itSeg.first);

                    itNear = near.insert({p1, itSeg.first});
                    if (itNear.second == false)
                        ((itNear.first)->second).insert(itSeg.first);
                }
            }
        }

        void addSegment(
            const std::pair<int, int> &idInput,
            const std::set<Point<T, 3>, PointCompare> &allP,
            std::set<Segment<T, 3>, SegmentCompare> &allSeg,
            std::map<Point<T, 3>,
                     std::set<typename std::set<Segment<T, 3>, SegmentCompare>::iterator>,
                     PointCompare> &near,
            const SegmentCompare &segCmp,
            const int mDim, Real tol = TOL)
        {
            for (auto itp0 = allP.begin(); itp0 != --allP.end(); ++itp0)
            {
                for (auto itp1 = std::next(itp0); itp1 != allP.end(); ++itp1)
                {

                    Segment<T, 3> newSeg(*itp0, *itp1, std::vector<std::pair<int, int>>(1, idInput));
                    int exist = false;
                    for (auto &&itSeg : near[*itp0])
                    {
                        if (segCmp.compare(*itSeg, newSeg))
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