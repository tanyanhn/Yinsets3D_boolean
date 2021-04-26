#ifndef SEGMENT_H
#define SEGMENT_H

#include "Point.h"
#include "Line.h"
#include <vector>
#include "PointCompare.h"
#include <set>

namespace YSB
{
    template <class T>
    struct FindNearTriangle;

    template <class T>
    struct RemoveOverlap;

    template <class T, int Dim>
    class Segment
    {
    public:
        enum locType
        {
            Inter = 0,
            ExtPoint,
            Outer
        };

        enum intsType
        {
            None = 0,
            One = 1,
            Overlap = 2,
            EndPoint = 3
        };

        friend struct FindNearTriangle<T>;
        friend struct RemoveOverlap<T>;

    protected:
        Point<T, Dim> endPoint[2];
        std::vector<std::pair<int, int>> neighbor;
        int intersectionSeg = 0; // Mark Intersection Line.

    public:
        Segment() = default;
        Segment(const Point<T, Dim> &stPoint, const Point<T, Dim> &fnPoint,
                std::vector<std::pair<int, int>> n = std::vector<std::pair<int, int>>())
            : neighbor(n)
        {
            endPoint[0] = stPoint;
            endPoint[1] = fnPoint;
        }
        Segment<T, Dim>(const Segment<T, Dim> &rhs)
            : neighbor(rhs.neighbor)
        {
            endPoint[0] = rhs.endPoint[0], endPoint[1] = rhs.endPoint[1];
            neighbor = rhs.neighbor;
            intersectionSeg = rhs.intersectionSeg;
        }
        Segment<T, Dim> &operator=(const Segment<T, Dim> &rhs)
        {
            endPoint[0] = rhs.endPoint[0], endPoint[1] = rhs.endPoint[1];
            neighbor = rhs.neighbor;
            intersectionSeg = rhs.intersectionSeg;
            return *this;
        }
        ~Segment() = default;

        // Accessors
        Point<T, Dim> &operator[](int _d) { return endPoint[_d]; }

        const Point<T, Dim> &operator[](int _d) const { return endPoint[_d]; }

        std::vector<std::pair<int, int>> &neighborhood() { return neighbor; }

        const std::vector<std::pair<int, int>> &neighborhood() const { return neighbor; }

        void addneighbor(std::pair<int, int> n) { neighbor.push_back(n); }

        int &IntersectionSeg() { return intersectionSeg; }

        const int &IntersectionSeg() const { return intersectionSeg; }

        //Combining neighbor
        void combineNeighbor(const Segment<T, Dim> &rhs)
        {
            std::set<std::pair<int, int>> newitem(neighbor.begin(), neighbor.end());
            for (auto &&id : rhs.neighbor)
            {
                newitem.insert(id);
            }
            neighbor.clear();
            for (auto &&id : newitem)
            {
                neighbor.push_back(id);
            }
        }

        // Get majorDim that largest change Dim
        int majorDim(int k = 1) const
        {
            // int md = 0;
            Vec<T, Dim> v = abs(endPoint[1] - endPoint[0]);
            return v.majorDim(k);
            // Real Lar = v[0];
            // for (auto d = 1; d < Dim; ++d)
            // {
            //     if (k * Lar < k * v[d])
            //     {
            //         md = d;
            //         Lar = v[d];
            //     }
            // }
            // return md;
        }

        // Projection
        Segment<T, Dim - 1> project(int d = -1) const
        {
            if (d == -1)
            {
                d = majorDim(-1);
            }
            else
                assert(d < Dim && "Segment::project d > Dim");
            auto stp = endPoint[0].project(d),
                 fnp = endPoint[1].project(d);
            return Segment<T, Dim - 1>(stp, fnp);
        }

        // Estimate Point position with segment.
        locType containPoint(const Point<T, Dim> &p, int mDim = -1, Real tol = TOL) const
        {
            if (mDim == -1)
                mDim = majorDim();

            PointCompare pointcmp(tol);
            if (pointcmp.compare(endPoint[0], p) == 0 || pointcmp.compare(endPoint[1], p) == 0)
            {
                return ExtPoint;
            }
            Vec<T, Dim> v1 = endPoint[1] - endPoint[0],
                        v2 = p - endPoint[0];
            Real area = norm(cross(v1, v2)),
                 bot = norm(v1);
            if (area / bot < tol)
            {
                if (p[mDim] >= std::min(endPoint[0][mDim], endPoint[1][mDim]) &&
                    p[mDim] <= std::max(endPoint[0][mDim], endPoint[1][mDim]))
                {
                    return Inter;
                }
            }
            return Outer;
        }

        Line<T, Dim> getLine() const
        {
            return Line<T, Dim>(endPoint[0], endPoint[1] - endPoint[0]);
        }

        // void dealLinefixpoint(Line<T, 2> &l2) const
        // {
        //     //  Make sure two fixpoint distance not to far to avoid
        //     Point<T, 2> p1 = endPoint[0], p2 = endPoint[1];
        //     Vec<T, 2> direction = p2 - p1;
        //     Real moveLength = norm(p1 - l2.fixpoint);
        //     l2.direction = normalize(l2.direction);
        //     auto fwp3 = l2.fixpoint + l2.direction * moveLength,
        //          bkp3 = l2.fixpoint + l2.direction * moveLength;
        //     Real fwdist = norm(p1 - fwp3),
        //          bkdist = norm(p2 - bkp3);
        //     if (moveLength > fwdist)
        //     {
        //         l2.fixpoint = fwp3;
        //     }
        //     if (moveLength > bkdist && fwdist > bkdist)
        //     {
        //         l2.fixpoint = bkp3;
        //     }
        //     l2.direction = l2.direction * norm(direction);
        // }

        friend std::ostream &operator<<(std::ostream &os, const Segment<T, Dim> &w)
        {
            os << w.endPoint[0] << "_" << w.endPoint[1];
            return os;
        }
    };

    template <class T, int Dim>
    inline typename Segment<T, 2>::intsType
    intersectSegLine(
        const Segment<T, Dim> &seg1, const Line<T, Dim> &l2, Real tol = TOL)
    {
        // int mDim = seg1.majorDim(-1);
        // auto proSeg1 = seg1.project(),
        //      proL2 = l2.project();

        int mDim1 = seg1.majorDim(1),
            mDim2 = l2.majorDim(1), mDim;
        for (int d = 0; d < Dim; ++d)
        {
            if (d != mDim1 && d != mDim2)
                mDim = d;
        }
        auto proSeg1 = seg1.project(mDim),
             proL2 = l2.project(mDim);

        return intersectSegLine(proSeg1, proL2, std::vector<Point<T, 2>>(), tol);
    }

    // intersect function in 2D, while parallel return 0, else return 1.
    template <class T>
    inline typename Segment<T, 2>::intsType
    intersectSegLine(const Segment<T, 2> &seg1, Line<T, 2> &l2, std::vector<Point<T, 2>> &result, Real tol = TOL)
    {
        Point<T, 2> p1 = seg1[0], p2 = seg1[1];

        // Deal Line's fixpoint too far
        int mDim = l2.majorDim();
        /*
        l2.moveFixpoint(seg1[0][mDim], mDim);
        Point<T, 2> p3 = l2.fixpoint;
	l2.moveFixpoint(seg1[1][mDim], mDim);
	Point<T, 2> p4 = l2.fixpoint;
*/
        l2.moveFixpoint(seg1[0][mDim], mDim);
        l2.direction = l2.direction * norm(p2 - p1);

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
                return Segment<T, 2>::intsType::None;
            result.push_back(p1);
            result.push_back(p2);
            return Segment<T, 2>::intsType::Overlap;
        }

        // solve for intersections by Cramer's rule
        Real x[2];
        x[0] = cross(b, A[1]) / det;
        x[1] = cross(A[0], b) / det;

        if (x[0] > -tol / sc && x[0] < 1 + tol / sc)
        {
            result.emplace_back(p1 + (p2 - p1) * x[0]);
            return Segment<T, 2>::intsType::One;
        }
        return Segment<T, 2>::intsType::None;
    }

    template <class T, int Dim>
    inline typename Segment<T, 2>::intsType
    intersectSegSeg(
        const Segment<T, Dim> &seg1, const Segment<T, Dim> &seg2,
        Real tol = TOL)
    {
        int mDim1 = seg1.majorDim(1),
            mDim2 = seg2.majorDim(1), mDim;
        for (int d = 0; d < Dim; ++d)
        {
            if (d != mDim1 && d != mDim2)
                mDim = d;
        }
        auto proSeg1 = seg1.project(mDim),
             proSeg2 = seg2.project(mDim);

        std::vector<Point<T, 2>> rs;

        return intersectSegSeg(proSeg1, proSeg2, rs, tol);
    }
    template <class T>
    inline typename Segment<T, 2>::intsType
    intersectSegSeg(
        const Segment<T, 2> &seg1, const Segment<T, 2> &seg2,
        std::vector<Point<T, 2>> &result, Real tol = TOL)
    {
        // intersect on endpoint.
        PointCompare cmp(tol);
        if (cmp.compare(seg1[0], seg2[0]) == 0)
        {
            if(seg1.containPoint(seg2[1]) == Segment<T, 2>::locType::Inter||
               seg2.containPoint(seg1[1]) == Segment<T, 2>::locType::Inter||
               cmp.compare(seg1[1], seg2[1]) == 0)
               return Segment<T, 2>::intsType::Overlap;
            result.push_back(seg1[0]);
            return Segment<T, 2>::intsType::EndPoint;
        }
        if (cmp.compare(seg1[0], seg2[1]) == 0)
        {
            if(seg1.containPoint(seg2[0]) == Segment<T, 2>::locType::Inter||
               seg2.containPoint(seg1[1]) == Segment<T, 2>::locType::Inter||
               cmp.compare(seg1[1], seg2[0]) == 0)
               return Segment<T, 2>::intsType::Overlap;
            result.push_back(seg1[0]);
            return Segment<T, 2>::intsType::EndPoint;
        }
        if (cmp.compare(seg1[1], seg2[0]) == 0)
        {
            if(seg1.containPoint(seg2[1]) == Segment<T, 2>::locType::Inter||
               seg2.containPoint(seg1[0]) == Segment<T, 2>::locType::Inter||
               cmp.compare(seg1[0], seg2[1]) == 0)
               return Segment<T, 2>::intsType::Overlap;
            result.push_back(seg1[1]);
            return Segment<T, 2>::intsType::EndPoint;
        }
        if (cmp.compare(seg1[1], seg2[1]) == 0)
        {
            if(seg1.containPoint(seg2[0]) == Segment<T, 2>::locType::Inter||
               seg2.containPoint(seg1[0]) == Segment<T, 2>::locType::Inter||
               cmp.compare(seg1[0], seg2[0]) == 0)
               return Segment<T, 2>::intsType::Overlap;
            result.push_back(seg1[1]);
            return Segment<T, 2>::intsType::EndPoint;
        }

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
                return Segment<T, 2>::intsType::None;
            return solveForOverlie(p1, p2, p3, p4, result, tol, seg1.majorDim());
        }

        // solve for intersections by Cramer's rule
        Real x[2];
        x[0] = cross(b, A[1]) / det;
        x[1] = cross(A[0], b) / det;

        if (x[0] > -tol / sc && x[0] < 1 + tol / sc && x[1] > -tol / sc && x[1] < 1 + tol / sc)
        {
            result.emplace_back(p1 + (p2 - p1) * x[0]);
            return Segment<T, 2>::intsType::One;
        }
        return Segment<T, 2>::intsType::None;
    }

    template <class T, int Dim>
    inline typename Segment<T, Dim>::intsType
    solveForOverlie(Point<T, Dim> &p1, Point<T, Dim> &p2,
                    Point<T, Dim> &p3, Point<T, Dim> &p4,
                    std::vector<Point<T, Dim>> &result,
                    Real tol, int majorDim)
    {
        if (p1[majorDim] > p2[majorDim])
            std::swap(p1, p2);
        if (p3[majorDim] > p4[majorDim])
            std::swap(p3, p4);
        auto min = (p1[majorDim] < p3[majorDim]) ? (p3) : (p1),
             max = (p2[majorDim] < p4[majorDim]) ? (p2) : (p4);
        Real r = max[majorDim] - min[majorDim];
        if (r < -tol)
        {
            return Segment<T, Dim>::intsType::None;
        }
        else if (r > tol)
        {
            result.push_back(min);
            result.push_back(max);
            return Segment<T, Dim>::intsType::Overlap;
        }
        result.push_back(min + (max - min) * 0.5);
        return Segment<T, Dim>::intsType::One;
    }

} // namespace YSB

#endif // !SEGMENT_H
