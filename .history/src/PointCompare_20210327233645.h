#ifndef POINTCOMPARE_H
#define POINTCOMPARE_H

#include "Point.h"

namespace YSB
{
    class PointCompare
    {
    protected:
        Real tol;

    public:
        PointCompare(Real t = TOL);
        ~PointCompare();
        template <class T1, class T2, int Dim>
        int operator()(const Point<T1, Dim> &lhs, const Point<T2, Dim> &rhs)
        {
            for (auto d = Dim - 1; d >= 0; --d)
            {
                if (std::abs(lhs[d] - rhs[d]) > tol)
                    return (lhs[d] < rhs[d]) ? (1) : (-1);
            }
            if (std::abs(lhs[1] - rhs[1]) <= tol)
            {
                if (std::abs(lhs[0] - rhs[0]) <= tol)
                    return 0;
                return (lhs[0] < rhs[0]) ? (-1) : (1);
            }
            return (lhs[1] > rhs[1]) ? (-1) : (1);
        }
    };

    PointCompare::PointCompare(Real t = TOL)
    {
        tol = t;
    }
} // namespace YSB

#endif // !POINTCOMPARE_H
