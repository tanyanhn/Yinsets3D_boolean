#ifndef PREPAST_H
#define PREPAST_H

#include "GluingCompactSurface.h"
#include "SurfacePatch.h"
#include "TriangleCompare.h"
#include "SegmentCompare.h"
#include <map>

namespace YSB
{
    template <class T>
    struct PrePast
    {
        std::vector<SurfacePatch<T>> vecSP;
        //std::vector<GluingCompactSurface<T>> vecGCS;

        void initialize()
        {
            vecSP.clear();
            //vecGCS.clear();
        }
        void operator()(std::vector<Triangle<T, 3>> &vecTri, const int inYinset, Real tol = TOL)
        {
            std::vector<int> F;
            std::vector<std::pair<int, int>> vecF;
            std::set<int> All;
            SegmentCompare cmp(tol);
            std::map<Segment<T, 3>,
                     std::vector<std::pair<int, int>>,
                     SegmentCompare>
                boundary(cmp);
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

            F.push_back((*All.begin()));
            All.erase(All.begin());
            while (!All.empty() || !F.empty())
            {
                Triangle<T, 3> &tri = vecTri[F.back()];
                vecF.emplace_back(inYinset, F.back());
                F.pop_back();
                tri.inF() = std::make_pair(inYinset, vecSP.size());

                for (int iEdge = 0; iEdge < 3; ++iEdge)
                {
                    const Segment<T, 3> &e = tri.ed(iEdge);
                    if (e.IntersectionSeg() == 0)
                    {
                        std::pair<int, int> nearTri;
                        nearTri = (e.neighborhood()[0].second != tri.id()) ? e.neighborhood()[1] : e.neighborhood()[0];
                        // if (inYinset == 1)
                        //     nearTri = FNToperator(tri, e, vecTri, std::vector<Triangle<T, 3>>(), tol);
                        // else if (inYinset == 2)
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
                                                                 std::vector<std::pair<int, int>>(1, std::make_pair(inYinset, tri.id()))));
                        if (it.second == false)
                        {
                            (it.first)->second.push_back({inYinset, tri.id()});
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