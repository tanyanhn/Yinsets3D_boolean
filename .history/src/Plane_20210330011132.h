#ifndef PLANE_H
#define PLANE_H

#include "Line.h"

namespace YSB
{
    // Plane in 3D space
    template <class T>
    struct Plane
    {
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

        ~Plane() = default;

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
        // Must have estimate parallel.
        Line<T, 3> intersect(const Plane &pl2, Real tol = TOL) const;

        // Plane intersect Line witch isn't parallel.
        Point<T, 3> intersect(const Line<T, 3> &l, Real tol = TOL) const;
    };

    template <class T>
    inline Point<T, 3> Plane<T>::intersect(const Line<T, 3> &l, Real tol) const
    {
        // Solving equations a_1 x + b_1 y + c_1 z = d_1              (1)
        // And the Line direction x / a = y / b = z / c.
        Real a = l.direction[0], b = l.direction[1], c = l.direction[2];
        Real a_1 = normVec[0], b_1 = normVec[1], c_1 = normVec[2], d_1;

        Real rs[3], x, y, z;

        if (std::abs(a) > tol)
        { // a != 0
            // Replace y = b / a * x, z = c / a * x.
            // Get k x = d_1.
            Real k = a_1 + b_1 * b / a + c_1 * c / a;
            x = d_1 / k;
            y = b / a * x;
            z = c / a * x;
        }
        else if (std::abs(b) > tol)
        { // b != 0
            // Replace x = a / b* y, z = c / b * y.
            // Get k y = d_1.
            Real k = a_1 * a / b + b_1 + c_1 * c / b;
            y = d_1 / k;
            x = a / b * y;
            z = c / b * y;
        }
        rs[0] = x;
        rs[1] = y;
        rs[2] = z;
        return Line<T, 3>(Point<T, 3>(rs), direction);
    }
}

template <class T>
inline Line<T, 3> Plane<T>::intersect(const Plane &pl2, Real tol) const
{
    // Cross product get line direction.
    Vec<T, 3> direction = normalize(cross(this->normVec, pl2.normVec));
    Real rs[3], x, y, z;
    // Solving equations a_1 x + b_1 y + c_1 z = d_1              (1)
    //                                        a_2 x + b_2 y + c_2 z = d_2              (2),
    // And the Line direction x / a = y / b = z / c.
    Real a = direction[0], b = direction[1], c = direction[2];

    Real a_1 = normVec[0], b_1 = normVec[1], c_1 = normVec[2], d_1,
         a_2 = pl2.normVec[0], b_2 = pl2.normVec[1], c_2 = pl2.normVec[2], d_2;
    d_1 = a_1 * fixpoint[0] + b_1 * fixpoint[1] + c_1 * fixpoint[2];
    d_2 = a_2 * pl2.fixpoint[0] + b_2 * pl2.fixpoint[1] + c_2 * pl2.fixpoint[2];

    if (std::abs(a) > tol)
    { // a != 0
        //  Let fixpoint[x] = 0
        x = 0;
        // Solving equations b_1 y + c_1 z = d_1              (3)
        //                                        b_2 y + c_2 z = d_2              (4),
        // c_2 * (1) - c_1 * (2) get k y = l
        Real k = c_2 * b_1 - c_1 * b_2;
        Real l = c_2 * d_1 - c_1 * d_2;

        y = l / k;
        if (std::abs(c_1) > tol)
        {
            z = (d_1 - b_1 * y) / c_1;
        }
        else if (std::abs(c_2) > tol)
        {
            z = (d_2 - b_2 * y) / c_2;
        }
        else
            assert(false && "Plane::intersect");
    }
    else if (std::abs(b) > tol)
    {
        //  Let fixpoint[y] = 0
        y = 0;
        // Solving equations a_1 x + c_1 z = d_1              (3)
        //                                        a_2 x + c_2 z = d_2              (4),
        // c_2 * (1) - c_1 * (2) get k x = l
        Real k = c_2 * a_1 - c_1 * a_2;
        Real l = c_2 * d_1 - c_1 * d_2;

        x = l / k;
        if (std::abs(c_1) > tol)
        {
            z = (d_1 - a_1 * x) / c_1;
        }
        else if (std::abs(c_2) > tol)
        {
            z = (d_2 - a_2 * x) / c_2;
        }
        else
            assert(false && "Plane::intersect");
    }
    else if (std::abs(c) > tol)
    {
        //  Let fixpoint[z] = 0
        z = 0;
        // Solving equations a_1 x + b_1 y= d_1              (3)
        //                                        a_2 x + b_2 y = d_2              (4),
        // b_2 * (1) - b_1 * (2) get k x = l
        Real k = b_2 * a_1 - b_1 * a_2;
        Real l = b_2 * d_1 - b_1 * d_2;

        x = l / k;
        if (std::abs(b_1) > tol)
        {
            y = (d_1 - a_1 * x) / b_1;
        }
        else if (std::abs(b_2) > tol)
        {
            y = (d_2 - a_2 * x) / b_2;
        }
        else
            assert(false && "Plane::intersect");
    }
    else
        assert(false && "Plane::intersect, if(a,b,c)");
    rs[0] = x;
    rs[1] = y;
    rs[2] = z;
    return Line<T, 3>(Point<T, 3>(rs), direction);
}

} // namespace YSB

#endif // !PLANE_H