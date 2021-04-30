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

        template <class T1>
        Line<T, Dim> &operator=(const Line<T1, Dim> &rhs)
        {
            fixpoint = rhs.fixpoint;
            direction = rhs.direction;
            return (*this);
        }

    public:
        // Projection
        Line<T, Dim - 1> project(int d) const
        {
            return Line<T, Dim - 1>(fixpoint.project(d), direction.project(d));
        }

        int majorDim(int k = 1) const
        {
            int md = 0;
            Vec<T, Dim> v = abs(direction);
            Real Lar = v[0];
            for (auto d = 1; d < Dim; ++d)
            {
                if (k * Lar < k * v[d])
                {
                    md = d;
                    Lar = v[d];
                }
            }
            return md;
        }

        void moveFixpoint(Real x, int d)
        {
            fixpoint = fixpoint + direction * (x - fixpoint[d]) / direction[d];
        }

        bool containPoint(const Point<T, Dim> &p, Real tol = TOL)
        {
            int mDim = this->majorDim();
            this->moveFixpoint(p[mDim], mDim);

            return norm(cross(p - fixpoint, direction)) / norm(direction) < tol;
        }
    };

} // namespace YSB

#endif // !LINE_H