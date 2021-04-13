#ifndef TRIANGLECOMPARE_H
#define TRIANGLECOMPARE_H

#include "Triangle.h"

namespace YSB
{
    class TriangleCompare
    {
    protected:
        Real tol;

    public:
        explicit TriangleCompare(Real t = TOL);
        ~TriangleCompare() = default;

        // Used for Point compare.
        template <class T1, class T2, int Dim>
        bool operator()(const Point<T1, Dim> &lhs, const Point<T2, Dim> &rhs)
        {
            return compare(lhs, rhs) == 1;
        }

        template <class T1, class T2, int Dim>
        int compare(const Point<T1, Dim> &lhs, const Point<T2, Dim> &rhs)
        {
            PointCompare cmp(tol);
            Point<T1, Dim> l[3];
            Point<T2, Dim> r[3];
            for (int i = 0; i < 3; ++i)
            {
                l[i] = lhs.vertex[i];
                r[i] = rhs.vertex[i];
            }

            if (cmp(l[0], l[1]))
                std::swap(l[0], l[1]);
            if (cmp(l[1], l[2]))
                std::swap(l[1], l[2]);
            if (cmp(l[0], l[1]))
                std::swap(l[0], l[1]);

            if (cmp(r[0], r[1]))
                std::swap(r[0], r[1]);
            if (cmp(r[1], r[2]))
                std::swap(r[1], r[2]);
            if (cmp(r[0], r[1]))
                std::swap(r[0], r[1]);

            int rs = cmp.compare(l[2], r[2]);
            if (rs == 0)
            {
                rs = cmp.compare(l[1], r[1]);
                if (rs == 0)
                {
                    rs = cmp.compare(l[0], r[0]);
                }
            }
            return rs;
        }
    };

    inline TriangleCompare::TriangleCompare(Real t)
    {
        tol = t;
    }
} // namespace YSB

#endif // !TRIANGLECOMPARE_H