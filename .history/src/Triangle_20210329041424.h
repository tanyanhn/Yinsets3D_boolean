#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Segment.h"
#include "Plane.h"

namespace YSB
{
    template <class T, int Dim>
    class Triangle
    {
    public:
        // rename
        using tPoint = Point<T, Dim>;
        using tSegment = Segment<T, Dim>;
        using tVec = Vec<T, Dim>;
        using tTriangle = Triangle<T, Dim>;

    private:
        tPoint vertex[3];
        tSegment edge[3];
        int inFace;
        mutable Plane<T> *pla;

    public:
        // Constructor
        Triangle() : inFace(-1), pla(nullptr) {}
        explicit Triangle(const tPoint *vecP, int f = -1)
            : inFace(f), pla(nullptr)
        {
            vertex[0] = vecP[0];
            vertex[1] = vecP[1];
            vertex[2] = vecP[2];
            edge[0] = tSegment(vertex[0], vertex[1]);
            edge[1] = tSegment(vertex[1], vertex[2]);
            edge[2] = tSegment(vertex[2], vertex[0]);
        }
        explicit Triangle(const tSegment *vecSeg, int f = -1)
            : inFace(f), pla(nullptr)
        {
            vertex[0] = vecSeg[0][0];
            vertex[1] = vecSeg[1][0];
            vertex[2] = vecSeg[2][0];
            edge[0] = vecSeg[0];
            edge[1] = vecSeg[1];
            edge[2] = vecSeg[2];
        }
        Triangle &operator=(const tTriangle &rhs)
        {
            vertex[0] = rhs.vertex[0];
            vertex[1] = rhs.vertex[1];
            vertex[2] = rhs.vertex[2];
            edge[0] = rhs.edge[0];
            edge[1] = rhs.edge[1];
            edge[2] = rhs.edge[2];
            inFace = rhs.inFace;
            if (pla != nullptr)
                delete pla;
            pla = new Plane<T>(*(rhs.pla));
            return *this;
        }
        Triangle(const tTriangle &rhs)
        {
            *this = rhs;
        }
        ~Triangle()
        {
            if (pla != nullptr)
                delete pla;
        };

        // Update pointer pla.
        Plane<T> *new_pla()
        {
            if (pla != nullptr)
                delete pla;
            pla = new Plane<T>(vertex[0], cross(vertex[1] - vertex[0], vertex[2] - vertex[0]));
            pla->direction = normalize(pla->direction) * perimeter();
            return pla;
        }

        Real perimeter() const
        {
            return norm(vertex[0] - vertex[1]) + norm(vertex[1] - vertex[2]) + norm(vertex[2] - vertex[0]);
        }

        bool parallel(const tTriangle &tri2) const
        {
            if (pla == nullptr)
                new_pla();
            if (tri2->pla == nullptr)
                tri2->new_pla();
        }
    };

} // namespace YSB

#endif // !TRIANGLE_H
