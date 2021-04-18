#ifndef TRIANGULATION_H
#define TRIANGULATION_H

#include "Triangle.h"
#include <map>

namespace YSB
{
    template <class T>
    struct Triangulation
    {
        std::map<int, std::vector<int>> TriangulateA, TriangulateB;
        //    std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA, resultB;
        std::vector<Triangle<T, 3>> vecTriA, vecTriB;

        void operator()(const std::vector<Triangle<T, 3>> &inputA,
                        const std::vector<Triangle<T, 3>> &inputB,
                        std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA,
                        std::map<int, std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultB,
                        Real tol = TOL)
        {
            this->operator()(inputA, vecTriA, TriangulateA, resultA, tol);
            this->operator()(inputB, vecTriB, TriangulateB, resultB, tol);
        }
        void operator()(const std::vector<Triangle<T, 3>> &input, std::vector<Triangle<T, 3>> vecTri,
                        std::map<int.std::vector<int>> Triangulate,
                        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> result, Real tol = TOL);
    };

} // namespace YSB

#endif // !TRIANGULATION_H