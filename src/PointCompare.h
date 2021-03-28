#ifndef POINTCOMPARE_H
#define POINTCOMPARE_H

#include "Point.h"

// Compare Points by lexicographic order

namespace YSB
{
    class PointCompare
    {
    protected:
        Real tol;

    public:
        PointCompare(Real t = TOL);
        ~PointCompare() = default;

        // Used for Point compare.
        template <class T1, class T2, int Dim>
        bool operator()(const Point<T1, Dim> &lhs, const Point<T2, Dim> &rhs)
        {
            return compare(lhs, rhs) == 1;
        }

        template <class T1, class T2, int Dim>
        int compare(const Point<T1, Dim> &lhs, const Point<T2, Dim> &rhs)
        {
            for (auto d = Dim - 1; d >= 0; --d)
            {
                if (std::abs(lhs[d] - rhs[d]) > tol)
                    return (lhs[d] < rhs[d]) ? (1) : (-1);
            }
            return 0;
        }
    };

    inline PointCompare::PointCompare(Real t)
    {
        tol = t;
    }
} // namespace YSB

#endif // !POINTCOMPARE_H
