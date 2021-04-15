#ifndef PAST_H
#define PAST_H

#include "SurfacePatch.h"
#include "GluingCompactSurface.h"
#include "FindNearTriangle.h"

namespace YSB
{
    template <class T>
    struct Past
    {
        GluingCompactSurface<T> vecGCS;

        void combine(const std::vector<SurfacePatch<T>> &vecFA,
                     const std::vector<SurfacePatch<T>> &vecFB,
                     const std::vector<SurfacePatch<T>> &vecF)
        {
        }

        void
        operator()(const std::vector<SurfacePatch<T>> &vecF,
                   const std::vector<Triangle<T, 3>> &vecTriA,
                   const std::vector<Triangle<T, 3>> &vecTriB, Real tol = TOL)
        {
            std::set<int> All;
            std::vector<int> pastF, connectF;
            FindNearTriangle<T> FNToperator;
            int size = vecF.size();
            std::vector<int> markF(size, 1);
            for (int i = 0; i < size; ++i)
            {
                if (!vecF[i].IfRemoved())
                    All.insert(i);
            }

            connectF.push_back((*All.begin()));
            All.erase(All.begin());
            while (!All.empty() && !connectF.empty())
            {
                const SurfacePatch<T> &SFP = vecF[connectF.back()];
                pastF.push_back(connectF.back());
                connectF.pop_back();

                for (auto &&it : SFP.bound())
                {
                    const Segment<T, 3> &e = it->first;
                    for (auto &&iTri : it->second)
                    {
                        const Triangle<T, 3> Tri, nearTri;
                        if (iTri.first == 1)
                            Tri = vecTriA[iTri.second];
                        else if (iTri.first == 2)
                            Tri = vecTriB[iTri.second];
                        else
                            assert(false && "Past::iTri wrong.");

                        std::pair<int, int> id_nearTri = FNToperator(Tri, e, vecTriA, vecTriB, tol);
                        if (id_nearTri.first == 1)
                            nearTri = vecTriA[id_nearTri.second];
                        else if (id_nearTri.first == 2)
                            nearTri = vecTriB[id_nearTri.second];
                        else
                            assert(false && "Past::id_nearTri wrong.");

                        if (markF[nearTri.inF()] == 1)
                        {
                            connectF.push_back(nearTri.inF());
                            All.erase(nearTri.inF());
                            markF[nearTri.inF()] = 0;
                        }
                    }

                    if (connectF.empty())
                    {
                        std::vector<std::pair<int, int>> id;
                        for (auto &&i : pastF)
                        {
                            id.insert(id.end(), vecF[i].tris().begin(), vecF[i].tris().end());
                        }
                        vecGCS.emplace_back(id, vecTriA, vecTriB, tol);

                        if (All.empty())
                            break;

                        connectF.push_back((*All.begin()));
                        All.erase(All.begin());
                    }
                }
            }
        }
    };

} // namespace YSB

#endif // !PAST_H