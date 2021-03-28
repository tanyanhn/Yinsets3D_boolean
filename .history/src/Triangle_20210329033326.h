#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Segment.h"

namespace YSB
{
    template <class T, int Dim>
    class Triangle
    {
    public:
        using tPoint = Point<T, Dim>;
        using tSegment = Segment<T, Dim>;
        using tVec = Vec<T, Dim>;

    private:
        tPoint vertex[3];
        tSegment edge[3];
        int inFace;

    public:
        explicit Triangle(const tPoint *vecP, int f = -1)
            : inFace(f)
        {
            vertex[0] = vecP[0];
            vertex[1] = vecP[1];
            vertex[2] = vecP[2];
            edge[0] = Segment(vertex[0], vertex[1]);
            edge[1] = Segment(vertex[1], vertex[2]);
            edge[2] = Segment(vertex[2], vertex[0]);
        }
        ~Triangle();
    };

    Triangle::Triangle(/* args */)
    {
    }

    Triangle::~Triangle()
    {
    }

} // namespace YSB

#endif // !TRIANGLE_H
