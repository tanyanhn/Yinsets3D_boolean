#ifndef SPACEDIVISION_H
#define SPACEDIVISION_H

#include "Cuboid.h"
#include "SegmentCompare.h"
#include "TriangleCompare.h"
#include <map>

namespace YSB
{

template<class T>
class SpaceDivision
{
 public:

    // find all the intersection in a Yinset.
    std::map<Segment<T, 3>, std::set<Triangle<T, 3>, TriangleCompare>, SegmentCompare> 
            intersectinner(std::vector<Triangle<T, 3>> vectri, Real tol = TOL );

    // determine if a triangle intersects the divided cuboids.
    std::vector<int> intersectDividedCuboid(Triangle<T, 3> tri, Cuboid<T> cub, Real tol = TOL);

};

template<class T>
inline std::map<Segment<T, 3>, std::set<Triangle<T, 3>, TriangleCompare>, SegmentCompare> 
            SpaceDivision<T>::intersectinner(std::vector<Triangle<T, 3>> vectri, Real tol)
{
    std::map<Segment<T, 3>, std::set<Triangle<T, 3>, TriangleCompare>, SegmentCompare> mapseg;
    for(auto &&tri : vectri)
    {
        for(int i=0;i<3;i++)
        {
            auto iter = mapseg.find(tri.ed(i));
            if(iter != mapseg.end())
            {
                iter->second.insert(tri);
            }
            else{
                std::set<Triangle<T, 3>, TriangleCompare> settri{tri};
                mapseg[tri.ed(i)] = settri;                
            }
        }
    }
    std::map<Segment<T, 3>, std::set<Triangle<T, 3>, TriangleCompare>, SegmentCompare> res;
    for(auto it = mapseg.begin() ; it!=mapseg.end() ; it++)
    {
        if(it->second.size()>2)
        {
            res[it->first] = it->second;
        }
    }
    return res;

}


template<class T>
inline std::vector<int> SpaceDivision<T>::intersectDividedCuboid(Triangle<T, 3> tri, Cuboid<T> cub, Real tol)
{
    std::vector<int> res(8,0);
    std::vector<Cuboid<T>> subcubs = cub.divide();
    std::vector<Segment<T, 3>> insres;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(subcubs[i].contain(tri.vert(j), tol))
            {
                res[i] = 1;
                continue;
            }
            insres.clear();
            subcubs[i].intersect(tri, insres, tol);
            if(insres.size()>0)
            res[i] = 1;

        
        }
    }
    return res;

}


}


#endif