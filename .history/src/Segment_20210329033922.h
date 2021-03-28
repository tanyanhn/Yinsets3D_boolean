#ifndef SEGMENT_H
#define SEGMENT_H

#include "Point.h"
#include "Line.h"
#include "Triangle.h"
#include <vector>
#include "PointCompare.h"

namespace YSB
{
    template <class T, int Dim>
    class Segment
    {
    public:
        using tPoint = Point<T, Dim>;
        using tVec = Vec<T, Dim>;
        using tSegment = Segment<T, Dim>;

    protected:
        tPoint endPoint[2];
        std::vector<int> neighbor;

    public:
        Segment() = default;
        Segment(const tPoint &stPoint, const tPoint &fnPoint)
        {
            endPoint[0] = stPoint;
            endPoint[1] = fnPoint;
        }
        Segment<T, Dim>(const Segment<T, Dim> &rhs) = default;
        Segment<T, Dim> operator=(const Segment<T, Dim> &rhs) = default;
        ~Segment() = default;

        // Projection
        Segment<T, Dim - 1> project(int d) const
        {
            assert(d < Dim && "Segment::project d > Dim");
            auto stp = endPoint[0]->project(d),
                 fnp = endPoint[1]->project(d);
            return Segment<T, Dim - 1>(stp, fnp);
        }

        // Accessors
        tPoint &operator[](int _d) { return endPoint[_d]; }

        const tPoint &operator[](int _d) const { return endPoint[_d]; }

        std::vector<int> &neighborhood() { return neighbor; }

        const std::vector<int> &neighborhood() const { return neighbor; }

        // Get majorDim that largest change Dim
        int majorDim() const
        {
            int md = 0;
            tVec v = abs(endPoint[1] - endPoint[0]);
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

        // Estimate Point position with segment.
        enum locType
        {
            Inter = 0,
            Extpoint,
            Outer
        };
        locType containPoint(const tPoint &p, int mDim, Real tol = TOL) const
        {
            PointCompare pointcmp;
            if (pointcmp(endPoint[0], p) || pointcmp(endPoint[1], p))
            {
                return Extpoint;
            }
            tVec v1 = endPoint[1] - endPoint[0],
                 v2 = p - endPoint[0];
            Real area = norm(cross(v1, v2)),
                 bot = norm(v1);
            if (area / bot < tol)
            {
                if (p[mDim] >= std::min(endPoint[0][mDim], endPoint[1][mDim]) && p[mDim] <= std::max(endPoint[0][mDim], endPoint[1][mDim]))
                {
                    return Inter;
                }
            }
            return Outer;
        }

        template <T>
        void dealLinefixpoint(Line<T, 2> &l2) const
        {
            //  Make sure two fixpoint distance not to far to avoid
            Point<T, 2> p1 = endPoint[0], p2 = endPoint[1];
            Vec<T, 2> direction = p2 - p1;
            Real moveLength = norm(p1 - l2.fixpoint);
            l2.direction = normalize(l2.direction);
            auto fwp3 = l2.fixpoint + l2.direction * moveLength,
                 bkp3 = l2.fixpoint + l2.direction * moveLength;
            Real fwdist = norm(p1 - fwp3),
                 bkdist = norm(p2 - bkp3);
            if (moveLength > fwdist)
            {
                l2.fixpoint = fwp3;
            }
            if (moveLength > bkdist && fwdist > bkdist)
            {
                l2.fixpoint = bkp3;
            }
            l2.direction = l2.direction * norm(direction);
        }
    };

    enum intsType
    {
        None = 0,
        One = 1,
        Overlap = 2
    };

    // intersect function in 2D, while parallel return 0, else return 1.
    template <class T>
    intsType intersect(const Segment<T, 2> &seg1, Line<T, 2> &l2, std::vector<Point<T, 2>> &result, Real tol = TOL)
    {
        Point<T, 2> p1 = seg1[0], p2 = seg1[1];

        // Deal Line's fixpoint too far
        seg1.dealLinefixpoint(l2);

        Point<T, 2> p3 = l2.fixpoint, p4 = l2.fixpoint + l2.direction;

        Vec<T, 2> A[2], b;
        A[0] = p2 - p1;
        A[1] = p3 - p4;
        b = p3 - p1;
        Real det = cross(A[0], A[1]);
        Real sc = norm(A[0]);

        if (std::abs(det / sc) < tol)
        { // parallel segments
            Real r = cross(A[0], b) / sc;
            if (std::abs(r) > tol)
                return intsType::None;
            result.push_back(p1);
            result.push_back(p2);
            return intsType::Overlap;
        }

        // solve for intersections by Cramer's rule
        Real x[2];
        x[0] = cross(b, A[1]) / det;
        x[1] = cross(A[0], b) / det;

        if (x[0] > -tol / sc && x[0] < 1 + tol / sc)
        {
            result.emplace_back(p1 + (p2 - p1) * x[0]);
            return intsType::One;
        }
        return intsType::None;
    }

    template <class T>
    intsType intersect(const Segment<T, 2> &seg1, const Segment<T, 2> &seg2, std::vector<Point<T, 2>> &result, Real tol = TOL)
    {
        Point<T, 2> p1 = seg1[0], p2 = seg1[1],
                    p3 = seg2[0], p4 = seg2[1];

        Vec<T, 2> A[2], b;
        A[0] = p2 - p1;
        A[1] = p3 - p4;
        b = p3 - p1;
        Real det = cross(A[0], A[1]);
        Real sc = norm(A[0]);

        if (std::abs(det / sc) < tol)
        { // parallel segments
            Real r = cross(A[0], b) / sc;
            if (std::abs(r) > tol)
                return intsType::None;
            return solveForOverlie(p1, p2, p3, p4, result, tol, seg1.majorDim());
        }

        // solve for intersections by Cramer's rule
        Real x[2];
        x[0] = cross(b, A[1]) / det;
        x[1] = cross(A[0], b) / det;

        if (x[0] > -tol / sc && x[0] < 1 + tol / sc && x[1] > -tol / sc && x[1] < 1 + tol / sc)
        {
            result.emplace_back(p1 + (p2 - p1) * x[0]);
            return intsType::One;
        }
        return intsType::None;
    }

    template <class T, int Dim>
    inline intsType
    solveForOverlie(Point<T, Dim> &p1, Vec<T, Dim> &p2,
                    Point<T, Dim> &p3, Point<T, Dim> &p4,
                    std::vector<Point<T, Dim>> &result,
                    Real tol, int majorDim)
    {
        if (p1[majorDim] > p2[majorDim])
            std::swap(p1, p2);
        if (p3[majorDim] > p4[majorDim])
            std::swap(p3, p4);
        result.push_back((p1[majorDim] < p3[majorDim]) ? (p3) : (p1));
        result.push_back((p2[majorDim] < p4[majorDim]) ? (p2) : (p4));
        Real r = result[1][majorDim] - result[0][majorDim];
        if (r < -tol)
        {
            return intsType::None;
        }
        else if (r > tol)
        {
            return intsType::Overlap;
        }
        result[0] = (result[0] + result[1]) * 0.5;
        return intsType::One;
    }

} // namespace YSB

#endif // !SEGMENT_H