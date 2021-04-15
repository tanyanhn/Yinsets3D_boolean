#ifndef SURFACEPATCH_H
#define SURFACEPATCH_H

#include "Triangle.h"
#include "SegmentCompare.h"
#include <map>

namespace YSB
{

    template <class T>
    class Locate;

    template <class T>
    class SurfacePatch
    {
        friend class Locate<T>;

    private:
        std::vector<std::pair<int, int>> vecTriangle;
        std::map<Segment<T, 3>, std::vector<std::pair<int, int>>> boundary;
        bool removed = false;

    public:
        SurfacePatch(const std::vector<std::pair<int, int>> &t,
                     std::vector<std::pair<Segment<T, 3>, std::vector<std::pair<int, int>>>> &b, Real tol = TOL)
            : vecTriangle(t)
        {
            SegmentCompare cmp(tol);

            for (auto ib = b.begin(); ib != b.end(); ++ib)
            {
                const auto insertRs = boundary.insert({ib->first, ib->second});
                if (!insertRs.second)
                {
                    ((insertRs.first)->second).insert(((insertRs.first)->second).end(), (ib->second).begin(), (ib->second).end());
                }
            }
        }

        //Accessors
        std::vector<std::pair<int, int>> &tris() { return vecTriangle; }

        const std::vector<std::pair<int, int>> &tris() const { return vecTriangle; }

        std::map<Segment<T, 3>, std::vector<std::pair<int, int>>> &bound() { return boundary; }

        const std::map<Segment<T, 3>, std::vector<std::pair<int, int>>> &bound() const { return boundary; }

        bool IfRemoved() const { return removed; }

        // void collapse(std::vector<std::pair<int,int>> &rs) const
        // {
        //     rs.insert(rs.end(), vecTriangle.begin(), vecTriangle.end());
        // }
        ~SurfacePatch();
    };

} // namespace YSB

#endif // !SURFACEPATCH_H