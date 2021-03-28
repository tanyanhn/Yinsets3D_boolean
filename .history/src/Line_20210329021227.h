#ifndef LINE_H
#define LINE_H

#include "Vec.h"
#include "Point.h"

namespace YSB
{
    template <class T, int Dim>
    struct Line
    {
        // A Point  and a direction can determine a straight Line.
        Point<T, Dim> fixpoint;
        Vec<T, Dim> direction;

    public:
        // Constructor
        template <class T1, class T2>
        Line(const Point<T1, Dim> &p, const Vec<T2, Dim> &d)
            : fixpoint(p), direction(d) {}
        ~Line() = default;

        template <class T1>
        explicit Line(const Line<T1, Dim> &rhs)
            : fixpoint(rhs.fixpoint), direction(rhs.direction) {}

    public:
        // Projection
        Line<T, Dim - 1> project(int d) const
        {
            return Line<T, Dim - 1>(fixpoint.project(d), direction.project(d));
        }
    };

    // intersect function in 2D, while parallel return 0, else return 1.
    template <class T>
    int intersect(const Line<T, 2> &l1, const Line<T, 2> &l2, Point<T, 2> &result, Real tol = TOL)
    {
        Point<T, 2> p1 = l1.fixpoint, p2 = l1.fixpoint + l1.direction;

        //  Make sure two fixpoint distance not to far to avoid
        Real moveLength = norm(p1 - l2.fixpoint);
        l2.direction = normalize(l2.direction);
        auto fwp3 = l2.fixpoint + l2.direction * moveLength,
             bkp3 = l2.fixpoint + l2.direction * moveLength;
        Real fwdist = norm(l1.fixpoint - fwp3),
             bkdist = norm(l1.fixpoint - bkp3);
        if (moveLength > fwdist)
        {
            l2.fixpoint = fwp3;
        }
        if (moveLength > bkdist && fwdist > bkdist)
        {
            l2.fixpoint = bkp3;
        }
        l2.direction = l2.direction * norm(l1.direction);
    }

} // namespace YSB

#endif // !LINE_H