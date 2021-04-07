#ifndef GLUINGCOMPACTSURFACE_H
#define GLUINGCOMPACTSURFACE_H

#include "Triangle.h"

namespace YSB
{
    enum FacType
    {
        Inner = 0,
        Outer = 1,
    };

    template <class T>
    class GluingCompactSurface
    {

    private:
        std::vector<Triangle<T, 3>> vecTriangle;
        FacType type;

    public:
        GluingCompactSurface(const std::vector<Triangle<T, 3>> &vt, int t = -1)
            : vecTriangle(vt)
        {
            if (t != -1)
                type = FacType(t);
            else
                calType();
        }

        void collapse(std::vector<Triangle<T, 3>> &rs) const
        {
            rs.insert(rs.end(), vecTriangle.begin(), vecTriangle.end());
        }

        FacType calType() const;

        FacType &Type() { return type; }

        const FacType &Type() const { return type; }

        ~GluingCompactSurface();
    };

} // namespace YSB

#endif // !GLUINGCOMPACTSURFACE_H