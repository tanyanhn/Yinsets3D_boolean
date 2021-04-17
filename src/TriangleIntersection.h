#ifndef TRIANGLEINTERSECTION_H
#define TRIANGLEINTERSECTION_H

#include "Triangle.h"

namespace YSB
{
    template <class T>
    struct TriangleIntersection
    {
        using intsType = typename Triangle<T, 3>::intsType;

        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> resultA, resultB;

        void operator()(const std::vector<Triangle<T, 3>> &inputA, const std::vector<Triangle<T, 3>> &inputB, Real tol = TOL);
    };

    template <class T>
    inline void TriangleIntersection<T>::operator()(const std::vector<Triangle<T, 3>> &inputA, const std::vector<Triangle<T, 3>> &inputB, Real tol)
    {
        int numA = inputA.size(),
            numB = inputB.size();
        resultA.reserve(numA);
        resultB.reserve(numB);

        // for (int iA = 0; iA < numA; ++iA)
        // {
        //     resultA.emplace_back(std::vector<Segment<T, 3>>{inputA[iA].ed(0), inputA[iA].ed(1), inputA[iA].ed(2)}, std::vector<int>());
        // }
        // for (int iB = 0; iB < numB; ++iB)
        // {
        //     resultB.emplace_back(std::vector<Segment<T, 3>>{inputB[iB].ed(0), inputB[iB].ed(1), inputB[iB].ed(2)}, std::vector<int>());
        // }

        std::vector<Segment<T, 3>> result;
        intsType type;

        for (int iA = 0; iA < numA + numB; ++iA)
        {
            for (int iB = iA + 1; iB < numA + numB; ++iB)
            {
                result.clear();
                int inYinsetA = iA < numA ? (1) : (2),
                    inYinsetB = iB < numA ? (1) : (2);
                if (inYinsetA == 1 && inYinsetB == 1)
                {
                    type = inputA[iA].intersect(inputA[iB], result, tol);
                }
                else if (inYinsetA == 1 && inYinsetB == 2)
                {
                    iB -= numA;
                    type = inputA[iA].intersect(inputB[iB], result, tol);
                }
                else if (inYinsetA == 2 && inYinsetB == 2)
                {
                    iA -= numA;
                    iB -= numA;
                    type = inputB[iA].intersect(inputB[iB], result, tol);
                }
                else
                {
                    assert(false && "TriangleIntersection::iA,iB.");
                }

                if (type == intsType::Never)
                {
                    continue;
                }
                else if (type == intsType::IntsPoint)
                {
                }
                else if (type == intsType::IntsSeg)
                {
                }
                else if (type == intsType::Overlap)
                {
                    if (inYinsetA == 1 && inYinsetB == 1)
                    {
                        resultA[iA].second.push_back({1, iB});
                        resultA[iB].second.push_back({1, iA});
                    }
                    else if (inYinsetA == 1 && inYinsetB == 2)
                    {
                        resultA[iA].second.push_back({2, iB});
                        resultB[iB].second.push_back({1, iA});
                    }
                    else if (inYinsetA == 2 && inYinsetB == 2)
                    {
                        resultB[iA].second.push_back({2, iB});
                        resultB[iB].second.push_back({2, iA});
                    }
                    else
                    {
                        assert(false && "TriangleIntersection::iA,iB.");
                    }
                }

                for (auto &&iSeg : result)
                {
                    iSeg.neighborhood().push_back(std::make_pair(inYinsetA, iA));
                    iSeg.neighborhood().push_back(std::make_pair(inYinsetB, iB));
                    //   iSeg.IntersectionSeg() = 1;

                    resultA[iA].first.push_back(iSeg);
                    resultB[iB].first.push_back(iSeg);
                }
            }
        }
    }

} // namespace YSB

#endif // !TRIANGLEINTERSECTION_H