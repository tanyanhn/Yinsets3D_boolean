#ifndef SEGMENT_H
#define SEGMENT_H

#include "Point.h"
#include "Line.h"
#include "Triangle.h"
#include <vector>

namespace YSB
{
    template <class T, int Dim>
    class Segment
    {
    public:
        using tPoint = Point<T, Dim>;

    protected:
        tPoint endPoint[2];
        std::vector<int> neighbor;

    public:
        Segment(const Point<T, Dim> &stPoint, const Point<T, Dim> &fnPoint)
        {
            endPoint[0] = stPoint;
            endPoint[1] = fnPoint;
        }
        Segment<T, Dim>(const Segment<T, Dim> &rhs) = default;
        Segment<T, Dim> operator=(const Segment<T, Dim> &rhs) = default;
        ~Segment();
    };

} // namespace YSB

#endif // !SEGMENT_H