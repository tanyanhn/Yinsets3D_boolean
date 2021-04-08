#ifndef REMOVEOVERLAP_H
#define REMOVEOVERLAP_H

#include "Triangle.h"
#include "Segment.h"

namespace YSB
{
    template <class T>
    struct RemoveOverlap
    {
        std::vector<std::pair<std::vector<Segment<T, 3>>, std::vector<int>>> resultA, resultB;
        std::vector<Triangle<T, 3>> vecTriA, vecTriB,
            inputA, inputB;
        std::vector<std::vector<int>> TriangulateA, TriangulateB;

        void operator()();
        void RemoveTriangle(typename std::vector<Triangle<T, 3>>::iterator it);
    };

    template <class T>
    inline void RemoveOverlap<T>::operator()()
    {
        int numA = inputA.size();
        for (int iA = 0; iA < numA; ++iA)
        {
            int numOverlap = resultA[iA].second.size();
            for (int iOverlap = 0; iOverlap < numOverlap; ++iOverlap)
            {
                int numsmalltriA = TriangulateA[iA].size(),
                    numsmalltriB = TriangulateB[iOverlap].size();
                for (int ismalltriA = 0; ismalltriA < numsmalltriA; ++ismalltriA)
                {
                    for (int ismalltriB = 0; ismalltriB < numsmalltriB; ++ismalltriB)
                    {
                        if (vecTriA[TriangulateA[iA][ismalltriA]].equal(vecTriB[TriangulateB[iOverlap][ismalltriB]]))
                        {
                            if (dot(vecTriA[TriangulateA[iA][ismalltriA]].normVec(),
                                    vecTriB[TriangulateB[iOverlap][ismalltriB]].normVec()) < 0)
                            {
                                RemoveTriangle(std::advance(vecTriA.begin(), TriangulateA[iA][ismalltriA]));
                            }
                            RemoveTriangle(std::advance(vecTriB.begin(), TriangulateB[iOverlap][ismalltriB]));
                        }
                    }
                }
            }
        }
    }

    template <class T>
    inline void RemoveOverlap<T>::RemoveTriangle(typename std::vector<Triangle<T, 3>>::iterator it)
    {
    }

} // namespace YSB

#endif // !REMOVEOVERLAP_H