#ifndef TRIANGLEINTERSECTION_H
#define TRIANGLEINTERSECTION_H

#include "Triangle.h"

namespace YSB
{
    template <class T>
    struct TriangleIntersection
    {
        std::vector<std::pair<std::vector<Segment<T, 3>, std::vector<int>>>> resultA,
            resultB;
    };

} // namespace YSB

#endif // !TRIANGLEINTERSECTION_H