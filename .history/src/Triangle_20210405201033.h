#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Segment.h"
#include "Plane.h"
#include <set>

namespace YSB
{
    template <class T, int Dim>
    class Triangle
    {
    public:
        // rename

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
            Overlap = 2,
            IntsPoint = 3,
            IntsSeg = 4
        };

    private:
        Point<T, Dim> vertex[3];
        Segment<T, Dim> edge[3];
        int inFace;
        mutable Plane<T> *pla;

    public:
        // Constructor
        Triangle() : inFace(-1), pla(nullptr) {}
        explicit Triangle(const Point<T, Dim> *vecP, int f = -1)
            : inFace(f), pla(nullptr)
        {
            vertex[0] = vecP[0];
            vertex[1] = vecP[1];
            vertex[2] = vecP[2];
            edge[0] = Segment<T, Dim>(vertex[0], vertex[1]);
            edge[1] = Segment<T, Dim>(vertex[1], vertex[2]);
            edge[2] = Segment<T, Dim>(vertex[2], vertex[0]);
        }
        explicit Triangle(const Segment<T, Dim> *vecSeg, int f = -1)
            : inFace(f), pla(nullptr)
        {
            vertex[0] = vecSeg[0][0];
            vertex[1] = vecSeg[1][0];
            vertex[2] = vecSeg[2][0];
            edge[0] = vecSeg[0];
            edge[1] = vecSeg[1];
            edge[2] = vecSeg[2];
        }
        Triangle &operator=(const Triangle<T, Dim> &rhs)
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
        Triangle(const Triangle<T, Dim> &rhs)
        {
            *this = rhs;
        }
        ~Triangle()
        {
            if (pla != nullptr)
                delete pla;
        };

        //Accessors
        Point<T, Dim> &vert(int i) { return vertex[i]; }
        const Point<T, Dim> &vert(int i) const { return vertex[i]; }

        Segment<T, Dim> &ed(int i) { return edge[i]; }
        const Segment<T, Dim> &ed(int i) const { return edge[i]; }

        // Update pointer pla.
        Plane<T> *new_pla() const
        {
            if (pla != nullptr)
                delete pla;
            pla = new Plane<T>(vertex[0], cross(vertex[1] - vertex[0], vertex[2] - vertex[0]));
            pla->normVec = normalize(pla->normVec) * perimeter();
            return pla;
        }

        Real perimeter() const
        {
            return norm(vertex[0] - vertex[1]) + norm(vertex[1] - vertex[2]) + norm(vertex[2] - vertex[0]);
        }

        bool isParallel(const Triangle<T, Dim> &tri2, Real tol = TOL) const
        {
            if (pla == nullptr)
                new_pla();
            if (tri2->pla == nullptr)
                tri2->new_pla();

            auto v1 = pla->normVec, v2 = tri2->pla->normVec;
            Real dist = norm(cross(v1, v2)) / norm(v1);
            return dist < tol;
        }

        int majorDim() const
        {
            if (pla == nullptr)
                new_pla();

            return pla->majorDim();
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
        int barycentric(const Point<T, Dim> &p, Real *co, Real tol = TOL) const
        {
            if (pla == nullptr)
                new_pla();
            Vec<T, Dim> A[3], E[3];
            A[0] = p - vertex[1], A[1] = p - vertex[2], A[2] = p - vertex[0];
            if (norm(dot(A[0], pla->normVec)) > tol)
            {
                return 0;
            }

            int m = 0;
            if (std::abs(pla->normVec[0]) < tol)
            {
                m = (std::abs(pla->normVec[1]) > std::abs(pla->normVec[2])) ? (1) : (2);
            }
            E[0] = vertex[2] - vertex[1];
            E[1] = vertex[0] - vertex[2];
            E[2] = vertex[1] - vertex[0];

            Vec<T, Dim> area = cross(E[0], E[1]), subarea;

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
            Vec<T, Dim> A[3];
            Point<T, Dim> zero(0);
            A[0] = vertex[0] - zero;
            A[1] = vertex[1] - zero;
            A[2] = vertex[2] = zero;

            return zero + A[0] * co[0] + A[1] * co[1] + A[2] * co[2];
        }

        // Locating Point with Triangle.
        Triangle<T, Dim>::locType locate(const Point<T, Dim> &p, Real tol = TOL) const
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
        Triangle<T, Dim>::intsType ifIntersect(const Triangle<T, Dim> &tri2, Real tol = TOL) const
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

            auto v1 = pla->normVec, v2 = tri2->pla->normVec;
            Real dist = norm(cross(v1, v2)) / norm(v1);
            if (dist < tol)
            { // parallel
                auto v = vertex[0] - tri2.vertex[0];
                if (norm(dot(v, pla->normVec)) < tol)
                    return intsType::Overlap;
                else
                    return intsType::Never;
            }

            return intsType::Maybe;
        }

        // Return number of intersection.
        int intersect(Line<T, Dim> &l, std::vector<Point<T, Dim>> &result, Real tol = TOL) const
        {
            if (pla == nullptr)
                new_pla();

            auto dir = normalize(l.direction);
            Real dist = norm(cross(dir, pla->normVec));
            if (dist < tol)
            { // parallel
                int mDim = majorDim();
                l.moveFixpoint(vertex[0][mDim], mDim);
                if (norm(dot(l.fixpoint - vertex[0], pla->normVec)) > tol)
                    return 0;

                auto projL = l.project(mDim);
                auto projTri = this->project(mDim);
                std::vector<Point<Real, 2>> rs2D;
                projTri.intersect(projL, rs2D, tol);
                for (auto ip : rs2D)
                {
                    Real co[3];
                    projTri.barycentric(ip, co, tol);
                    result.emplace_back(this->barycentric(co));
                }
                return rs2D.size();
            }

            auto p = pla->intersect(l, tol);
            if (this->locate(p, tol) != intsType::Outer)
            {
                result.push_back(p);
                return 1;
            }
            return 0;
        }

        intsType intersect(const Triangle<T, Dim> &tri2, std::vector<Segment<T, Dim>> &result, Real tol = TOL) const
        {
            if (pla == nullptr)
                new_pla();
            if (tri2->pla == nullptr)
                tri2->new_pla();

            intsType intsT = ifIntersect(tri2, tol);

            if (intsT == intsType ::Overlap)
            {
                int mDim = majorDim();
                auto projTri1 = project(mDim),
                     projTri2 = project(mDim);

                std::vector<Segment<Real, 2>> rs2D;
                projTri1.intersect(projTri2, rs2D, tol);
                for (auto ip : rs2D)
                {
                    Real stCo[3], fnCo[3];
                    projTri1.barycentric(ip[0], stCo, tol);
                    projTri1.barycentric(ip[1], fnCo, tol);
                    result.emplace_back(Segment<T, Dim>(this->barycentric(stCo), this->barycentric(fnCo)));
                }
                return intsType::Overlap;
            }
            else if (intsT == Never)
            {
                return intsType::Never;
            }
            else if (intsT == Maybe)
            {
                Line<T, Dim> l = this->pla->intersect(tri2.pla);
                std::vector<Point<T, Dim>> pts;

                int mDim = majorDim();
                auto projL = l.project(mDim);
                auto projTri = this->project(mDim);
                std::vector<Point<Real, 2>> rs2D;
                projTri.intersect(projL, rs2D, tol);
                if (rs2D.size() == 0)
                    return intsType::Never;
                for (auto ip : rs2D)
                {
                    Real co[3];
                    projTri.barycentric(ip, co, tol);
                    pts.emplace_back(this->barycentric(co));
                }
                if (rs2D.size() == 1)
                {
                    pts.push_back(pts[0]);
                }

                mDim = tri2.majorDim();
                projL = l.project(mDim);
                projTri = this->project(mDim);
                rs2D.clear();
                projTri.intersect(projL, rs2D, tol);
                if (rs2D.size() == 0)
                    return intsType::Never;
                for (auto ip : rs2D)
                {
                    Real co[3];
                    projTri.barycentric(ip, co, tol);
                    pts.emplace_back(tri2.barycentric(co));
                }
                if (rs2D.size() == 1)
                {
                    pts.push_back(pts[2]);
                }

                std::vector<Point<T, Dim>> rsEndp;
                typename Segment<T, Dim>::intsType segintsT = solveForOverlie(pts[0], pts[1], pts[2], pts[3], rsEndp, tol, l.majorDim());

                if (segintsT == Segment<T, Dim>::One)
                {
                    result.emplace_back(Segment<T, Dim>(rsEndp[0], rsEndp[0]));
                    return intsType::IntsPoint;
                }
                else if (segintsT == Segment<T, Dim>::Overlap)
                {
                    result.emplace_back(Segment<T, Dim>(rsEndp[0], rsEndp[1]));
                    return intsType::IntsSeg;
                }
                else
                    return intsType::Never;
            }
        }
    };

    template <>
    inline int Triangle<Real, 2>::intersect(Line<Real, 2> &l, std::vector<Point<Real, 2>> &result, Real tol) const
    {
        for (auto i = 0; i < 3; ++i)
        {
            intersectSegLine(edge[i], l, result, tol);
        }
        std::set<Point<Real, 2>, PointCompare> setp;
        for (auto ip : result)
        {
            setp.insert(ip);
        }
        result.clear();
        for (auto ip : setp)
        {
            result.push_back(ip);
        }
        return 1;
    }

    template <>
    inline int Triangle<Real, 2>::barycentric(const Point<Real, 2> &p, Real *co, Real) const
    {
        Vec<Real, 2> A[3], E[3];
        A[0] = p - vertex[1], A[1] = p - vertex[2], A[2] = p - vertex[0];

        E[0] = vertex[2] - vertex[1];
        E[1] = vertex[0] - vertex[2];
        E[2] = vertex[1] - vertex[0];

        Real area = cross(E[0], E[1]), subarea;

        for (auto i = 0; i < 3; ++i)
        {
            subarea = cross(E[i], A[i]);
            co[i] = (subarea / area);
        }
        return 1;
    }

    template <>
    inline Triangle<Real, 2>::intsType Triangle<Real, 2>::intersect(const Triangle<Real, 2> &tri2, std::vector<Segment<Real, 2>> &result, Real tol) const
    {
        for (auto i = 0; i < 3; ++i)
        {
            Line<Real, 2> l = tri2.edge[i].getLine();

            std::vector<Point<Real, 2>> rs2D;
            this->intersect(l, rs2D, tol);
            if (rs2D.size() == 0)
                break;
            else if (rs2D.size() == 1)
                rs2D.push_back(rs2D[0]);

            rs2D.push_back(tri2.edge[i][0]);
            rs2D.push_back(tri2.edge[i][1]);

            std::vector<Point<Real, 2>> rsEndp;
            typename Segment<Real, 2>::intsType segintsT = YSB::solveForOverlie(rs2D[0], rs2D[1], rs2D[2], rs2D[3], rsEndp, tol, l.majorDim());

            if (segintsT == Segment<Real, 2>::One)
            {
                result.emplace_back(Segment<Real, 2>(rsEndp[0], rsEndp[0]));
            }
            else if (segintsT == Segment<Real, 2>::Overlap)
            {
                result.emplace_back(Segment<Real, 2>(rsEndp[0], rsEndp[1]));
            }
            else
                break;
        }

        for (auto i = 0; i < 3; ++i)
        {
            Line<Real, 2> l = this->edge[i].getLine();

            std::vector<Point<Real, 2>> rs2D;
            tri2.intersect(l, rs2D, tol);
            if (rs2D.size() == 0)
                break;
            else if (rs2D.size() == 1)
                rs2D.push_back(rs2D[0]);

            rs2D.push_back(this->edge[i][0]);
            rs2D.push_back(this->edge[i][1]);

            std::vector<Point<Real, 2>> rsEndp;
            typename Segment<Real, 2>::intsType segintsT = YSB::solveForOverlie(rs2D[0], rs2D[1], rs2D[2], rs2D[3], rsEndp, tol, l.majorDim());

            if (segintsT == Segment<Real, 2>::One)
            {
                result.emplace_back(Segment<Real, 2>(rsEndp[0], rsEndp[0]));
            }
            else if (segintsT == Segment<Real, 2>::Overlap)
            {
                result.emplace_back(Segment<Real, 2>(rsEndp[0], rsEndp[1]));
            }
            else
                break;
        }

        return Overlap;
    }

} // namespace YSB

#endif // !TRIANGLE_H
