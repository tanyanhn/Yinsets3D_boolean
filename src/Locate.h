#ifndef LOCATE_H
#define LOCATE_H
#include "Triangle.h"
#include <vector>
#include <cstdlib>

namespace YSB
{
    template<class T>
    struct Locate
    {
        int operator()(const std::vector<Triangle<T, 3>> &yinset, const Point<T, 3> &p, Real tol = TOL);
    };


    template<class T>
    inline T RandomGenerate()
    {
        return -1.0 + 2.0 * rand()/T(RAND_MAX);
    }

    template<class T>
    inline Vec<T, 3> RandomDirection()
    {
        T a[3];
        for(int i=0;i<3;i++)
        a[i] = RandomGenerate<T>();
        Vec<T, 3> res(a);
        return res;
    }

    template<class T>
    inline int Locate<T>::operator()(const std::vector<Triangle<T, 3>> &yinset, const Point<T, 3> &p, Real tol)
    {
        // 1: in the interior of the yinset; 0: on the boundary of the yinset; -1: in the exterior of the yinset 
        while(1)
        {
            Vec<T, 3> dir = RandomDirection<T>();
            Line<T, 3> l(p, dir);
            std::vector<Point<T, 3>> intsp;
            std::vector<int> record;

            for(int i = 0; i < yinset.size(); i++)
            {
                auto tri = yinset[i];
                std::vector<Point<T, 3>> tmpres;
                if(tri.intersect(l, tmpres) == 1)
                {
                    if(norm(p-tmpres[0]) < tol)
                    return 0;
                    
                    intsp.push_back(tmpres[0]);
                    record.push_back(i);
                }
                else if(tri.intersect(l, tmpres) == 2)
                {
                    Segment<T, 3> s(tmpres[0], tmpres[1]);
                    if(s.containPoint(p, s.majorDim()) != Segment<T, 3>::locType::Outer)
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

            if(intsp.size() == 0)
            return -1;

            T min_D = 999999999.0;
            Point<T, 3> tmpp;
            int index;
            for(int i = 0; i < intsp.size(); i++)
            {
                auto ip = intsp[i];
                if(norm(ip-p) < min_D)
                {
                    tmpp = ip;
                    min_D = norm(ip-p);
                    index = i;
                }
            }

            if(yinset[record[index]].locate(tmpp) == Triangle<T, 3>::locType::Inter)
            {
                auto pla = yinset[record[index]].new_pla();
                Vec<T, 3> norm = pla->normVec;
                return (dot(norm, tmpp - p) > 0 ? 1 : -1);
            }

        }

    }
}














#endif