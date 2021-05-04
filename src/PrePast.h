#ifndef PREPAST_H
#define PREPAST_H

#include "GluingCompactSurface.h"
#include "SurfacePatch.h"
#include "TriangleCompare.h"
#include "SegmentCompare.h"
//#include "FindNearTriangle.h"
#include <map>

namespace YSB
{
    template <class T>
    struct PrePast
    {
        std::vector<SurfacePatch<T>> vecSPA, vecSPB;
        std::map<std::pair<int, int>, std::set<std::pair<int, int>>> ClipFaces;
        std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> coClipFaces;
        //std::vector<GluingCompactSurface<T>> vecGCS;

        void operator()(
            std::vector<Triangle<T, 3>> &vecTriA,
            std::vector<Triangle<T, 3>> &vecTriB,
            Real tol = TOL)
        {
            this->operator()(vecTriA, 1, tol);
            this->operator()(vecTriB, 2, tol);
        }

        void operator()(
            std::vector<Triangle<T, 3>> &vecTriA,
            Real tol = TOL)
        {
            this->operator()(vecTriA, 1, tol);
        }

        void operator()(
            std::vector<Triangle<T, 3>> &vecTri,
            const int idYinset, Real tol = TOL)
        {
            std::vector<SurfacePatch<T>> *vecSPAB[2] = {&vecSPA, &vecSPB};
            std::vector<SurfacePatch<T>> &vecSP = *(vecSPAB[(idYinset - 1)]);
            std::vector<int> F;
            std::vector<std::pair<int, int>> vecF;
            std::set<int> All;
            SegmentCompare cmp(tol);
            std::map<Segment<T, 3>,
                     std::vector<std::pair<int, int>>,
                     SegmentCompare>
                boundary(cmp);
            //FindNearTriangle<T> FNTOp;
            //FindNearTriangle<T> FNToperator;
            int size = vecTri.size();
            std::vector<int> markF(size, 1);
            for (int i = 0; i < size; ++i)
            {
                if (!vecTri[i].IfRemoved())
                {
                    vecTri[i].id(i - vecTri[i].id());
                    All.insert(i);
                }
            }
            if (!All.empty())
            {
                F.push_back((*All.begin()));
                All.erase(All.begin());
            }

            while (!All.empty() || !F.empty())
            {
                Triangle<T, 3> &tri = vecTri[F.back()];
                vecF.emplace_back(idYinset, F.back());
                markF[tri.id()] = 0;
                F.pop_back();
                ClipFaces[tri.inF()].insert({idYinset, vecSP.size()});
                coClipFaces[{idYinset, vecSP.size()}].push_back(tri.inF());
                tri.inF() = std::make_pair(idYinset, vecSP.size());

                for (int iEdge = 0; iEdge < 3; ++iEdge)
                {
                    const Segment<T, 3> &e = tri.ed(iEdge);
                    if (e.IntersectionSeg() == 0)
                    {
                        std::pair<int, int> nearTri;

                        assert(e.neighborhood().size() == 2 && "Not a closed surface");
                        nearTri = (e.neighborhood()[0].second != tri.id()) ? e.neighborhood()[0] : e.neighborhood()[1];
                        //     nearTri = FNToperator(tri, e, vecTri, std::vector<Triangle<T, 3>>(), tol);
                        // else if (idYinset == 2)
                        //     nearTri = FNToperator(tri, e, std::vector<Triangle<T, 3>>(), vecTri, tol);

                        if (markF[nearTri.second] == 1)
                        {
                            F.push_back(nearTri.second);
                            All.erase(nearTri.second);
                            markF[nearTri.second] = 0;
                        }
                    }
                    else
                    {
                        auto it = boundary.insert(std::make_pair(e,
                                                                 std::vector<std::pair<int, int>>(1, std::make_pair(idYinset, tri.id()))));
                        if (it.second == false)
                        {
                            (it.first)->second.push_back({idYinset, tri.id()});
                        }
                    }
                }

                if (F.empty())
                {
                    vecSP.emplace_back(vecF, boundary);
                    boundary.clear();
                    vecF.clear();

                    if (All.empty())
                        break;

                    F.push_back((*All.begin()));
                    All.erase(All.begin());
                }
            }
        }
    };
} // namespace YSB
#endif // !PREPAST_H