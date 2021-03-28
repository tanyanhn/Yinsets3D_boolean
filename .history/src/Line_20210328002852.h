#ifndef LINE_H
#define LINE_H

#include "Vec.h"
#include "Point.h"

namespace YSB
{
    template <class T, int Dim>
    class Line
    {
    protected:
        // A Point  and a direction can determine a straight Line.
        Point<T, Dim> fixpoint;
        Vec<T, Dim> direction;

    public:
        // Constructor
        template <class T1, class T2>
        Line(const Point<T1, Dim> &p, const Vec<T2, Dim> &d)
            : fixpoint(p), direction(d) {}
        ~Line();

        template <class T1>
        explicit Line(const Line<T1, Dim> &rhs)
            : fixpoint(rhs.fixpoint), direction(rhs.direction) {}

    private:
        // Projection
        Line<T, Dim - 1> project(int d)
        {
            return Line<T, Dim - 1>(fixpoint.project(d), direction.project(d));
        }
    };

} // namespace YSB

#endif // !LINE_H