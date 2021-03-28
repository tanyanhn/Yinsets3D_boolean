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
            }
        }
    };

} // namespace YSB

#endif // !SEGMENT_H