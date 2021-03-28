#ifndef PLANE_H
#define PLANE_H

#include "Line.h"

namespace YSB
{
    // Plane in 3D space
    template <class T>
    class Plane
    {
    private:
        //  Norm vector and a fixpoint unique determine a plane.
        Point<T, spaceDim> fixpoint;
        Vec<T, spaceDim> normVec;

    public:
        // Constructor
        template <class T1, class T2>
        Plane(const Point<T1, spaceDim> &p, const Vec<T2, spaceDim> &d)
            : fixpoint(p), normVec(d) {}

        template <class T2>
        explicit Plane(const Plane<T2> &pl)
            : fixpoint(pl.fixpoint), normVec(pl.normVec) {}

        ~Plane();

        //  Get properly project dimension
        int properProject() const
        {
            int rs = 0;
            Real value = std::abs(normVec[0]);
            for (auto d = 1; d < 3; ++d)
            {
                Real nVd = std::abs(normVec[d]);
                if (value < nVd)
                {
                    rs = d;
                    value = nVd;
                }
            }
            return rs;
        }

        // Plane intersect get Line in 3D space.
        Line<T, 3> intersect(const Plane &pl2) const
        {
            // Cross product get line direction.
            Vec<T, 3> direction = normalize(cross(this->normVec, pl2.normVec));
            // Solving equations a_1 x + b_1 y + c_1 z = d_1                     (1)
            //                                        a_2 x + b_2 y + c_2 z = d_2              (2),
            // And the Line direction x / a = y / b = z / c.
            Real a = direction[0], b = direction[1], c = direction[2];

            Real a1 = normVec[0], b1 = normVec[1], c1 = normVec[2], d1,
                 a2 = pl2.normVec[0], b2 = pl2.normVec[1], c2 = pl2.normVec[2], d2;
            d1 = a1 * fixpoint[0] + b1 * fixpoint[1] + c1 * fixpoint[2];
            d2 = a2 * pl2.fixpoint[0] + b2 * pl2.fixpoint[1] + c2 * pl2.fixpoint[2];

            if ()
        }
    };

} // namespace YSB

#endif // !PLANE_H