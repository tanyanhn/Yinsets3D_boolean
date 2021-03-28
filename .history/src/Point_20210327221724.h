#ifndef POINT_H
#define POINT_H

#include "Vec.h"
// Define Point in Euclidean space

namespace YSB
{

template<class T, int Dim>
class Point
{
private:
    Real coord[Dim]
public:
    Point(const T &t = T(){
        for(auto i = 0; i < Dim; ++i) coord[i] = t;
    }
    Point(std::initializer_list)
    ~Point();
};


    
} // namespace YSB

#endif // !POINT_H