#ifndef TRIANGLEINTERSECTION_H
#define TRIANGLEINTERSECTION_H

#include "Triangle.h"
#include "map"

namespace YSB
{
    template <class T>
    struct TriangleIntersection
    {
        using intsType = typename Triangle<T, 3>::intsType;

        // std::map<int, std::pair<std::vector<Segment<T, 3>>,
        //                         std::vector<std::pair<int, int>>>>
        //     resultA, resultB;
        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> resultA;
        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>> resultB;

        void operator()(const std::vector<Triangle<T, 3>> &inputA, const std::vector<Triangle<T, 3>> &inputB, Real tol = TOL);
    };

    template <class T>
    inline void TriangleIntersection<T>::operator()(const std::vector<Triangle<T, 3>> &inputA, const std::vector<Triangle<T, 3>> &inputB, Real tol)
    {
        int numA = inputA.size(),
            numB = inputB.size();
        resultA.resize(numA);
        resultB.resize(numB);
        // resultA.reserve(numA);
        // resultB.reserve(numB);

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

        for (int idA = 0; idA < numA + numB; ++idA)
        {
            for (int idB = idA + 1; idB < numA + numB; ++idB)
            {
                // if(idA==103&&idB==104)
                // {
                //     int a= idA;
                // }
                int iA, iB;
                result.clear();
                int inYinsetA = idA < numA ? (1) : (2),
                    inYinsetB = idB < numA ? (1) : (2);
                if (inYinsetA == 1 && inYinsetB == 1)
                {
                    iA = idA;
                    iB = idB;

                    type = inputA[iA].intersect(inputA[iB], result, tol);
                }
                else if (inYinsetA == 1 && inYinsetB == 2)
                {
                    iA = idA;
                    iB = idB - numA;
                    type = inputA[iA].intersect(inputB[iB], result, tol);
                }
                else if (inYinsetA == 2 && inYinsetB == 2)
                {
                    iA = idA - numA;
                    iB = idB - numA;
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

                    if (inYinsetA == 1 && inYinsetB == 1)
                    {
                        resultA[iA].first.push_back(iSeg);
                        resultA[iB].first.push_back(iSeg);
                    }
                    else if (inYinsetA == 1 && inYinsetB == 2)
                    {
                        resultA[iA].first.push_back(iSeg);
                        resultB[iB].first.push_back(iSeg);
                    }
                    else if (inYinsetA == 2 && inYinsetB == 2)
                    {
                        resultB[iA].first.push_back(iSeg);
                        resultB[iB].first.push_back(iSeg);
                    }
                    else
                    {
                        assert(false && "TriangleIntersection::iA,iB.");
                    }
                }
            }
        }
    }

} // namespace YSB

#endif // !TRIANGLEINTERSECTION_H