#ifndef SURFACEPATCH_H
#define SURFACEPATCH_H

#include "Triangle.h"

namespace YSB
{

    template <class T>
    class SurfacePatch
    {
    private:
        std::vector<std::pair<int, int>> vecTriangle;
        std::vector<std::pair<Segment<T, 3>, std::vector<std::pair<int, int>>>> boundary;

    public:
        SurfacePatch(const std::vector<std::pair<int, int>> &t,
                     std::vector<std::pair<Segment<T, 3>, std::vector<std::pair<int, int>>>> &b)
            : vecTriangle(t), boundary(b) {}

        // void collapse(std::vector<std::pair<int,int>> &rs) const
        // {
        //     rs.insert(rs.end(), vecTriangle.begin(), vecTriangle.end());
        // }
        ~SurfacePatch();
    };

} // namespace YSB

#endif // !SURFACEPATCH_H