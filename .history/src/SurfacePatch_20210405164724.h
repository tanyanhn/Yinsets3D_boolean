#ifndef SURFACEPATCH_H
#define SURFACEPATCH_H

#include "Triangle.h"

namespace YSB
{

    template <class T>
    class SurfacePatch
    {
    private:
        std::vector<Triangle<T, 3>> vecTriangle public : SurfacePatch(/* args */);
        ~SurfacePatch();
    };

    SurfacePatch::SurfacePatch(/* args */)
    {
    }

    SurfacePatch::~SurfacePatch()
    {
    }

} // namespace YSB

#endif // !SURFACEPATCH_H