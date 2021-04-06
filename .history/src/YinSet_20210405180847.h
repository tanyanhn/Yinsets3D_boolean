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

        void BuildHasse() ~YinSet();
    };

} // namespace YSB

#endif // !YINSET_H