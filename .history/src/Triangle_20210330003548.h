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
        enum locType
        {
            Inter = 0,
            InEdge,
            InVertex,
            Outer,
        };

        enum intsType
        {
            Never = 0,
            Maybe = 1,
            Overlap = 2
        };

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

        //Accessors
        tPoint &vert(int i) { return vertex[i]; }
        const tPoint &vert(int i) const { return vertex[i]; }

        tSegment &ed(int i) { return edge[i]; }
        const tSegment &ed(int i) const { return edge[i]; }

        // Update pointer pla.
        Plane<T> *new_pla() const
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

        bool isParallel(const tTriangle &tri2, Real tol = TOL) const
        {
            if (pla == nullptr)
                new_pla();
            if (tri2->pla == nullptr)
                tri2->new_pla();

            auto v1 = pla->direction, v2 = tri2->pla->direction;
            Real dist = norm(cross(v1, v2)) / norm(v1);
            return dist < tol;
        }

        int majorDim() const
        {
            if (pla == nullptr)
                new_pla();

            int md = 0;
            tVec v = abs(pla->direction);
            Real Lar = v[0];
            for (auto d = 1; d < Dim; ++d)
            {
                if (Lar < v[d])
                {
                    md = d;
                    Lar = v[d];
                }
            }
            return md;
        }

        Triangle<T, Dim - 1> project(int d) const
        {
            Point<T, Dim - 1> v[3];
            v[0] = vertex[0]->project(d);
            v[1] = vertex[1]->project(d);
            v[2] = vertex[2]->project(d);
            return Triangle<T, Dim - 1>(v);
        }

        // Calculating barycentric coordinates
        int barycentric(const tPoint &p, Real *co, Real tol = TOL) const
        {
            if (pla == nullptr)
                new_pla();
            tVec A[3], E[3];
            A[0] = p - vertex[1], A[1] = p - vertex[2], A[2] = p - vertex[0];
            if (norm(dot(A[0], pla->direction)) > tol)
            {
                return 0;
            }

            int m = 0;
            if (std::abs(pla->direction[0]) < tol)
            {
                m = (std::abs(pla->direction[1]) > std::abs(pla->direction[2])) ? (1) : (2);
            }
            E[0] = vertex[2] - vertex[1];
            E[1] = vertex[0] - vertex[2];
            E[2] = vertex[1] - vertex[0];

            tVec area = cross(E[0], E[1]), subarea;

            for (auto i = 0; i < 3; ++i)
            {
                subarea = cross(E[i], A[i]);
                co[i] = (subarea / area)[m];
            }
            return 1;
        }

        // Get Point from its barycentric.
        Point<T, Dim> barycentric(Real *co) const
        {
            tVec A[3];
            tPoint zero(0);
            A[0] = vertex[0] - zero;
            A[1] = vertex[1] - zero;
            A[2] = vertex[2] = zero;

            return zero + A[0] * co[0] + A[1] * co[1] + A[2] * co[2];
        }

        // Locating Point with Triangle.
        Triangle<T, Dim>::locType locate(const tPoint &p, Real tol = TOL) const
        {
            Real co[3];
            if (barycentric(p, co, tol) == 0)
                assert(false && "Triangle::locate(p), p not in the plane.");

            int countzero = 0;
            for (auto d = 0; d < Dim; ++d)
            {
                if (co[d] < -tol)
                    return locType::Outer;
                if (std::abs(co[d]) < tol)
                    ++countzero;
            }

            if (countzero == 0)
                return locType::Inter;
            else if (countzero == 1)
                return locType::InEdge;
            else if (countzero == 2)
                return locType::InVertex;
            else
                assert(false && "barycentric shouldn't have 3 zero.");
        }

        // Estimate two Triangle whether intersect.
        Triangle<T, Dim>::intsType ifIntersect(const tTriangle &tri2, Real tol = TOl) const
        {
            Real min1, min2, max1, max2;

            for (auto d = 0; d < Dim; ++d)
            {
                min1 = std::min(vertex[0][d], min(vertex[1][d], vertex[2][d]));
                max2 = std::min(tri2.vertex[0][d], max(tri2.vertex[1][d], tri2.vertex[2][d]));
                if (min1 > max2)
                    return intsType::Never;
                max1 = std::max(vertex[0][d], max(vertex[1][d], vertex[2][d]));
                min2 = std::min(tri2.vertex[0][d], min(tri2.vertex[1][d], tri2.vertex[2][d]));
                if (min2 > max1)
                    return intsType::Never;
            }
            if (pla == nullptr)
                new_pla();
            if (tri2->pla == nullptr)
                tri2->new_pla();

            auto v1 = pla->direction, v2 = tri2->pla->direction;
            Real dist = norm(cross(v1, v2)) / norm(v1);
            if (dist < tol)
            { // parallel
                auto v = vertex[0] - tri2.vertex[0];
                if (norm(dot(v, pla->direction)) < tol)
                    return intsType::Overlap;
                else
                    return intsType::Never;
            }

            return intsType::Maybe;
        }

        int intersect(const Line<T, Dim> &l, std::vector<tPoint> &result, Real tol = TOL) const
        {
            if (pla == nullptr)
                new_pla();

            auto dir = normalize(l.direction);
            Real dist = norm(cross(dir, pla->direction));
            if (dist < tol)
            { // parallel
                int mDim = majorDim();
                auto projL = l.project(mDim);
                auto projTri = this->project(mDim);
            }
        }
    };

} // namespace YSB

#endif // !TRIANGLE_H
