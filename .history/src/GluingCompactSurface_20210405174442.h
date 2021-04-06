#ifndef GLUINGCOMPACTSURFACE_H
#define GLUINGCOMPACTSURFACE_H

#include "Triangle.h"

namespace YSB
{
    enum FacType
    {
        inner = 0,
        outer
    }

    template <class T>
    class GluingCompactSurface
    {

    private:
        std::vector<Triangle<T, 3>> vecTriangle;
        FacType type;

    public:
        GluingCompactSurface();

        FacType ~GluingCompactSurface();
    };

} // namespace YSB

#endif // !GLUINGCOMPACTSURFACE_H