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
        int numA = inputA.size(),
            numB = inputB.size();
        resultA.resize(numA);
        resultB.resize(numB);
        for (int iA = 0; iA < numA; ++iA)
        {
        }

        for (int iA = 0; iA < numA; ++iA)
        {

            for (int iB = 0; iB < numB; ++iB)
            {
            }
        }
    }

} // namespace YSB

#endif // !TRIANGLEINTERSECTION_H