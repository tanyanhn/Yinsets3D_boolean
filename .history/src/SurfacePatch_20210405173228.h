#ifndef SURFACEPATCH_H
#define SURFACEPATCH_H

#include "Triangle.h"

namespace YSB
{

    template <class T>
    class SurfacePatch
    {
    private:
        std::vector<Triangle<T, 3>> vecTriangle;
        std::vector<std::pair<Segment<T, 3>, std::vector<int>>> boundary;

    public:
        SurfacePatch(const std::vector<Triangle<T, 3>> &t, std::vector<std::pair<Segment<T, 3>, std::vector<int>>> &b)
            : vecTriangle(t), boundary(b) {}

        void collapse(std::vector<Triangle<T, 3>> &rs)
        {
            rs.insert(rs.end(), vecTriangle.begin(), vecTriangle.end());
        }
        ~SurfacePatch();
    };

} // namespace YSB

#endif // !SURFACEPATCH_H