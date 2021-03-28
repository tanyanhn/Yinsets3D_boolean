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
    T coord[Dim]
public:
// Constructor
    Point(const T &t = T(){
        for(auto i = 0; i < Dim; ++i) coord[i] = t;
    }

    Point(std::initializer_list<T> l){
        assert(l.size() == Dim && "Point's initializer_list size not equal to Dim.");
        auto j = l.begin();
        for(auto d = 0; d < Dim; coord[d++] = *j++);
    }

    Point(const T const* l){
        for(auto d = 0; d < Dim; ++d){
            coord[d] = l[d];
        }
    }

    template<class T2>
    explicit Point(const Point<T2,Dim>& rhs){
        for(auto d = 0; d < Dim; d++)  coord[d] = rhs.coord[d];
    }

    // Accessors
    T &operator[](int _d) { return coord[_d]; }

    const T &operator[](int _d) const { return coord[_d];  }

    const T *data() const { return &coord[0]; }

    // Destructor
    ~Point();

    // Operator with Vec
    template<class T2>
    Vec<T,Dim> operator-(const Point<T2,Dim> &rhs){
        T rs[Dim];
        for (auto d = 0; d < Dim; d++)
        {
            rs[d] = coord[d] - rhs.coord[d];
        }
        return Vec<T,Dim>(rs);
    }
};


    
} // namespace YSB

#endif // !POINT_H