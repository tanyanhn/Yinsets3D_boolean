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
        }
    };

} // namespace YSB

#endif // !PLANE_H