#ifndef LOCATE_H
#define LOCATE_H
#include "Triangle.h"
#include "SurfacePatch.h"
#include "RemoveOverlap.h"
#include <vector>
#include <cstdlib>

namespace YSB
{
    template <class T>
    struct Locate
    {
        int operator()(const std::vector<Triangle<T, 3>> &yinset, const Point<T, 3> &p, Real tol = TOL);
        int operator()(const std::vector<Triangle<T, 3>> &yinset, const SurfacePatch<T> &faces, Real tol = TOL);
        void operator()(const std::vector<Triangle<T, 3>> &inputA, const std::vector<Triangle<T, 3>> &inputB,
                        std::vector<Triangle<T, 3>> &vecTriA, std::vector<Triangle<T, 3>> &vecTriB,
                        std::vector<SurfacePatch<T>> &vecSPA, std::vector<SurfacePatch<T>> &vecSPB, Real tol = TOL);
    };

    template <class T>
    inline T RandomGenerate()
    {
        return -1.0 + 2.0 * rand() / T(RAND_MAX);
    }

    template <class T>
    inline Vec<T, 3> RandomDirection()
    {
        T a[3];
        for (int i = 0; i < 3; i++)
            a[i] = RandomGenerate<T>();
        Vec<T, 3> res(a);
        return res;
    }

    template <class T>
    inline int Locate<T>::operator()(const std::vector<Triangle<T, 3>> &yinset, const Point<T, 3> &p, Real tol)
    {
        // 1: in the interior of the yinset; 0: on the boundary of the yinset; -1: in the exterior of the yinset
        while (1)
        {
            Vec<T, 3> dir = RandomDirection<T>();
            Line<T, 3> l(p, dir);
            std::vector<Point<T, 3>> intsp;
            std::vector<int> record;

            for (int i = 0; i < yinset.size(); i++)
            {
                auto tri = yinset[i];
                std::vector<Point<T, 3>> tmpres;
                if (tri.intersect(l, tmpres) == 1)
                {
                    if (norm(p - tmpres[0]) < tol)
                        return 0;

                    intsp.push_back(tmpres[0]);
                    record.push_back(i);
                }
                else if (tri.intersect(l, tmpres) == 2)
                {
                    Segment<T, 3> s(tmpres[0], tmpres[1]);
                    if (s.containPoint(p, s.majorDim()) != Segment<T, 3>::locType::Outer)
                        return 0;
                    else
                    {
                        intsp.push_back(tmpres[0]);
                        intsp.push_back(tmpres[1]);
                        record.push_back(i);
                        record.push_back(i);
                    }
                }
            }

            if (intsp.size() == 0)
                return -1;

            T min_D = YSB::GreatValue;
            Point<T, 3> tmpp;
            int index;
            for (int i = 0; i < intsp.size(); i++)
            {
                auto ip = intsp[i];
                if (norm(ip - p) < min_D)
                {
                    tmpp = ip;
                    min_D = norm(ip - p);
                    index = i;
                }
            }

            if (yinset[record[index]].locate(tmpp) == Triangle<T, 3>::locType::Inter)
            {
                auto pla = yinset[record[index]].new_pla();
                Vec<T, 3> norm = pla->normVec;
                return (dot(norm, tmpp - p) > 0 ? 1 : -1);
            }
        }
    }

    template <class T>
    inline int Locate<T>::operator()(
        const std::vector<Triangle<T, 3>> &yinset,
        const SurfacePatch<T> &face, Real tol)
    {
        const Triangle<T, 3> &tri = face.tris()[0];
        Point<T, 3> zero(0);

        Point<T, 3> barycenter = zero +
                                 ((tri.vert(0) - zero) + (tri.vert(1) - zero) + (tri.vert(2) - zero)) / 3;

        int rs = this->operator()(yinset, barycenter, tol);
        assert(rs != 0 && "Locate shouldn't on Surface.");
        return rs;
    }

    template <class T>
    inline void Locate<T>::operator()(
        const std::vector<Triangle<T, 3>> &inputA,
        const std::vector<Triangle<T, 3>> &inputB,
        std::vector<Triangle<T, 3>> &vecTriA,
        std::vector<Triangle<T, 3>> &vecTriB,
        std::vector<SurfacePatch<T>> &vecSPA,
        std::vector<SurfacePatch<T>> &vecSPB, Real tol)
    {
        for (auto &&iSP : vecSPA)
        {
            int k = this->operator()(inputB, iSP, tol);
            if (k == -1)
            {
                iSP.removed = true;
                for (auto &&it : iSP.tris())
                {
                    RemoveTriangle(vecTriA, vecTriB, vecTriA, it.second);
                }
            }
            else if (k == 1)
                ;
            else if (k == 0)
                assert(false && "Locate SurfacePatch have wrong.");
        }

        for (auto &&iSP : vecSPB)
        {
            int k = this->operator()(inputA, iSP, tol);
            if (k == -1)
            {
                iSP.removed = true;
                for (auto &&it : iSP.tris())
                {
                    RemoveTriangle(vecTriA, vecTriB, vecTriB, it.second);
                }
            }
            else if (k == 1)
                ;
            else if (k == 0)
                assert(false && "Locate SurfacePatch have wrong.");
        }
    }
}

#endif