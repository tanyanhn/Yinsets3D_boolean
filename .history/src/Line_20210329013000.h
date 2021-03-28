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
        // Solving equations
        // (x - x_1) / a_1 = (y - y_1) / b_1
        // (x - x_2) / a_2 = (y - y_2) / b_2
        Real a_1 = l1.direction[0], b_1 = l1.direction[1],
             a_2 = l2.direction[0], b_2 = l2.direction[1],
             x_1 = l1.fixpoint[0], y_1 = l1.fixpoint[1],
             x_2 = l2.direction[0], y_2 = l2.direction[1];
    }

} // namespace YSB

#endif // !LINE_H