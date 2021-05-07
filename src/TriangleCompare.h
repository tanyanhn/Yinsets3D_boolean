#ifndef TRIANGLECOMPARE_H
#define TRIANGLECOMPARE_H

#include "Triangle.h"

namespace YSB
{
    class TriangleCompare
    {
    protected:
        Real tol;
        int orientation = 0;

    public:
        explicit TriangleCompare(Real t = TOL, int ori = 0);
        ~TriangleCompare() = default;

        // Used for Point compare.
        template <class T1, class T2, int Dim>
        bool operator()(const Triangle<T1, Dim> &lhs, const Triangle<T2, Dim> &rhs) const
        {
            return compare(lhs, rhs) == 1;
        }

        template <class T1, class T2, int Dim>
        int compare(const Triangle<T1, Dim> &lhs, const Triangle<T2, Dim> &rhs) const
        {
            PointCompare cmp(tol);
            Point<T1, Dim> l[3];
            Point<T2, Dim> r[3];
            for (int i = 0; i < 3; ++i)
            {
                l[i] = lhs.vert(i);
                r[i] = rhs.vert(i);
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

            if (orientation == 1 && rs == 0)
            {
                int d = 0;
                for (int i = 0; i < 3; ++i)
                {
                    if (cmp.compare(lhs.vert(0), rhs.vert(i)) == 0)
                        d = i;
                }
                d += 1;
                if (d == 3)
                    d = 0;

                rs = cmp.compare(lhs.vert(1), rhs.vert(d));
            }
            return rs;
        }
    };

    inline TriangleCompare::TriangleCompare(Real t, int ori)
    {
        tol = t;
        orientation = ori;
    }
} // namespace YSB

#endif // !TRIANGLECOMPARE_H