#ifndef YINSET_H
#define YINSET_H
#include "GluingCompactSurface.h"

namespace YSB
{
    /// A node in the Hasse diagram.
    struct Node
    {
        int depth; // even number for positive orientation, odd number of negative orientation
        int parent;
        std::vector<int> children;
    };

    template <class T>
    class YinSet
    {
    private:
        std::vector<GluingCompactSurface<T>> vecFace;
        std::vector<Node> HasseMap;

    public:
        YinSet(const std::vector<GluingCompactSurface<T>> &vf)
            : vecFace(vf)
        {
            BuildHasse();
        }

        void collapse(std::vector<Triangle<T, 3>> &rs) const
        {
            for (auto &&iFace : vecFace)
            {
                iFace.collapse(rs);
            }
        }

        void BuildHasse() const;

        ~YinSet();
    };

} // namespace YSB

#endif // !YINSET_H