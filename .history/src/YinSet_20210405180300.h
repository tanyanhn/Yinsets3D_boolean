#ifndef YINSET_H
#define YINSET_H
#include "GluingCompactSurface.h"

namespace YSB
{
    template <class T>
    class YinSet
    {
    public:
        /// A node in the Hasse diagram.
        struct Node
        {
            int depth; // even number for positive orientation, odd number of negative orientation
            int parent;
            std::vector<int> children;
        };

    private:
        std::vector<GluingCompactSurface<T>> vecFace;

    public:
        YinSet(/* args */);
        ~YinSet();
    };

    YinSet::YinSet(/* args */)
    {
    }

    YinSet::~YinSet()
    {
    }

} // namespace YSB

#endif // !YINSET_H