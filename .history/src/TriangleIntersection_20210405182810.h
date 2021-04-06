#ifndef TRIANGLEINTERSECTION_H
#define TRIANGLEINTERSECTION_H

#include "Triangle.h"

namespace YSB
{
    template <class T>
    struct TriangleIntersection
    {
        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA, resultB;

        void operator()(const std::vector<Triangle<T, 3>> &inputA, const std::vector<Triangle<T, 3>> &inputB);
    };

    template <class T>
    inline void TriangleIntersection<T>::operator()(const std::vector<Triangle<T, 3>> &inputA, const std::vector<Triangle<T, 3>> &inputB)
    {
    }

} // namespace YSB

#endif // !TRIANGLEINTERSECTION_H