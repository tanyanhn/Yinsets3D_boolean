#ifndef TRIANGLEINTERSECTION_H
#define TRIANGLEINTERSECTION_H

#include "Triangle.h"
#include <map>
#include "TriangleCompare.h"

namespace YSB
{
    template <class T>
    struct TriangleIntersection
    {
        using intsType = typename Triangle<T, 3>::intsType;

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
        const std::vector<Triangle<T, 3>> *inputArr[2] = {&inputA, &inputB};
        std::vector<std::pair<std::vector<Segment<T, 3>>,
                              std::vector<std::pair<int, int>>>>
            *resultArr[2] = {&resultA, &resultB};
        PointCompare pCmp(tol);
        TriangleCompare triCmp(tol);
        std::map<Point<T, 3>,
                 std::vector<std::pair<int, int>>, PointCompare>
            mt(pCmp), ml(pCmp);
        std::set<std::pair<int, int>> interTris;
        std::set<Point<T, 3>, PointCompare> allP(pCmp);
        Point<T, 3> min, max;

        for (int idA = 0; idA < numA; ++idA)
        {
            if (pCmp.compare(inputA[idA].vert(0), inputA[idA].vert(1)) == 1)
            {
                min = inputA[idA].vert(0);
                max = inputA[idA].vert(1);
            }
            else
            {
                min = inputA[idA].vert(1);
                max = inputA[idA].vert(0);
            }
            min = (pCmp.compare(min, inputA[idA].vert(2)) == 1)
                      ? (min)
                      : inputA[idA].vert(2);
            max = (pCmp.compare(max, inputA[idA].vert(2)) == -1)
                      ? (max)
                      : inputA[idA].vert(2);

            allP.insert(min);
            allP.insert(max);

            auto it = mt.find(max);
            if (it == mt.end())
                mt.insert({max, std::vector<std::pair<int, int>>(1, {1, idA})});
            else
                it->second.push_back({1, idA});

            it = ml.find(min);
            if (it == ml.end())
                ml.insert({min, std::vector<std::pair<int, int>>(1, {1, idA})});
            else
                it->second.push_back({1, idA});
        }

        for (int idB = 0; idB < numB; ++idB)
        {
            if (pCmp.compare(inputB[idB].vert(0), inputB[idB].vert(1)) == 1)
            {
                min = inputB[idB].vert(0);
                max = inputB[idB].vert(1);
            }
            else
            {
                min = inputB[idB].vert(1);
                max = inputB[idB].vert(0);
            }
            min = (pCmp.compare(min, inputB[idB].vert(2)) == 1)
                      ? (min)
                      : inputB[idB].vert(2);
            max = (pCmp.compare(max, inputB[idB].vert(2)) == -1)
                      ? (max)
                      : inputB[idB].vert(2);

            allP.insert(min);
            allP.insert(max);

            auto it = mt.find(max);
            if (it == mt.end())
                mt.insert({max, std::vector<std::pair<int, int>>(1, {2, idB})});
            else
                it->second.push_back({2, idB});

            it = ml.find(min);
            if (it == ml.end())
                ml.insert({min, std::vector<std::pair<int, int>>(1, {2, idB})});
            else
                it->second.push_back({2, idB});
        }

        std::vector<Segment<T, 3>> result;
        intsType type;

        for (auto &&p : allP)
        {
            auto &maxP = mt[p];
            for (auto &&idTri : maxP)
                interTris.erase(idTri);

            auto &minP = ml[p];
            for (auto &&idTri1 : minP)
            {
                for (auto &&idTri2 : interTris)
                {
                    int iA = idTri1.second,
                        iB = idTri2.second;
                    result.clear();
                    int inYinsetA = idTri1.first,
                        inYinsetB = idTri2.first;
                    if (inYinsetA == 1 && inYinsetB == 1)
                    {
                        type = inputA[iA].intersect(inputA[iB], result, tol);
                    }
                    else if (inYinsetA == 1 && inYinsetB == 2)
                    {
                        type = inputA[iA].intersect(inputB[iB], result, tol);
                    }
                    else if (inYinsetA == 2 && inYinsetB == 1)
                    {
                        type = inputB[iA].intersect(inputA[iB], result, tol);
                    }
                    else if (inYinsetA == 2 && inYinsetB == 2)
                    {
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
                        else if (inYinsetA == 2 && inYinsetB == 1)
                        {
                            resultB[iA].second.push_back({1, iB});
                            resultA[iB].second.push_back({2, iA});
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
                        else if (inYinsetA == 2 && inYinsetB == 1)
                        {
                            resultB[iA].first.push_back(iSeg);
                            resultA[iB].first.push_back(iSeg);
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

                interTris.insert(idTri1);
            }
        }
    }

} // namespace YSB

#endif // !TRIANGLEINTERSECTION_H