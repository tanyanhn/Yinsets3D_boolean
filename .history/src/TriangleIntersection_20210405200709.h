#ifndef TRIANGLEINTERSECTION_H
#define TRIANGLEINTERSECTION_H

#include "Triangle.h"

namespace YSB
{
    template <class T>
    struct TriangleIntersection
    {
        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA, resultB;

        void operator()(const std::vector<Triangle<T, 3>> &inputA, const std::vector<Triangle<T, 3>> &inputB, Real tol = TOL);
    };

    template <class T>
    inline void TriangleIntersection<T>::operator()(const std::vector<Triangle<T, 3>> &inputA, const std::vector<Triangle<T, 3>> &inputB.Real tol)
    {
        int numA = inputA.size(),
            numB = inputB.size();
        resultA.reserve(numA);
        resultB.reserve(numB);

        for (int iA = 0; iA < numA; ++iA)
        {
            resultA.emplace_back(std::vector<Segment<T, 3>>{inputA[iA].ed(0), inputA[iA].ed(1), inputA[iA].ed(2)}, std::vector<int>());
        }
        for (int iB = 0; iB < numB; ++iB)
        {
            resultB.emplace_back(std::vector<Segment<T, 3>>{inputB[iB].ed(0), inputB[iB].ed(1), inputB[iB].ed(2)}, std::vector<int>());
        }

        std::vector<Segment<T, 3>> result;
        typename Triangle<T, 3>::intsType type;

        for (int iA = 0; iA < numA; ++iA)
        {
            for (int iB = 0; iB < numB; ++iB)
            {
                type = inputA[iA].intersect(inputB[iB], result, tol)
            }
        }
    }

} // namespace YSB

#endif // !TRIANGLEINTERSECTION_H