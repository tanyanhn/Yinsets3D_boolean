#ifndef PREPAST_H
#define PREPAST_H

#include "GluingCompactSurface.h"
#include "SurfacePatch.h"
#include "TriangleCompare.h"

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
            std::vector<std::pair<Segment<T, 3>, std::vector<std::pair<int, int>>>> boundary;
            FindNearTriangle<T> FNToperator;
            int size = vecTri.size();
            std::vector<int> markF(size, 1);
            for (int i = 0; i < size; ++i)
            {
                if (!vecTri[i].IfRemoved())
                {
                    vecTri[i].id(i - vecTri.id());
                    All.insert(i);
                }
            }

            F.push_back((*All.begin()));
            All.erase(All.begin());
            while (!All.empty() && !F.empty())
            {
                const Triangle<T, 3> &tri = vecTri[F.back()];
                vecF.emplace_back(inYinset, F.back());
                F.pop_back();
                tri.inF() = {inYinset, vecSP.size()};

                for (int iEdge = 0; iEdge < 3; ++iEdge)
                {
                    const Segment<T, 3> &e = tri.ed(iEdge);
                    if (e.IntersectionSeg() != 1)
                    {
                        std::pair<int, int> nearTri;
                        if (inYinset == 1)
                            nearTri = FNToperator(tri, e, vecTri, std::vector<Triangle<T, 3>>(), tol);
                        else if (inYinset == 2)
                            nearTri = FNToperator(tri, e, std::vector<Triangle<T, 3>>(), vecTri, tol);

                        if (markF[nearTri.second] == 1)
                        {
                            F.push_back(nearTri.second);
                            All.erase(nearTri.second);
                            markF[nearTri.second] = 0;
                        }
                    }
                    else
                    {
                        boundary.emplace_back(e, std::vector<std::pair<int, int>>(1, make_pair(inYinset, tri.id())), tol);
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