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
            if (std::abs(p1[1] - p2[1]) <= tol)
            {
                if (std::abs(p1[0] - p2[0]) <= tol)
                    return 0;
                return (p1[0] < p2[0]) ? (-1) : (1);
            }
            return (p1[1] > p2[1]) ? (-1) : (1);
        }
    };

    PointCompare::PointCompare(Real t = TOL)
    {
        tol = t;
    }
} // namespace YSB

#endif // !POINTCOMPARE_H
