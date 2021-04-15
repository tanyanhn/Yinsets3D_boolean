#ifndef YINSET_H
#define YINSET_H
#include "GluingCompactSurface.h"
#include "SegmentCompare.h"
#include "TriangleIntersection.h"
#include "Triangulation.h"
#include "RemoveOverlap.h"
#include "Locate.h"
#include "PrePast.h"
#include "Past.h"

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

        void collapse(std::vector<Triangle<T, 3>> &rs, int idYinset, Real tol = TOL) const
        {
            SegmentCompare segcmp(tol);

            std::map<Segment<T, 3>, std::vector<std::pair<int, int>>> segs(segcmp);
            for (auto &&iFace : vecFace)
            {
                iFace.collapse(rs, segs, idYinset, tol);
            }

            for (auto &&it : segs)
            {
                for (auto &&iTri : it.second)
                {
                    int ie = rs[iTri].edgeVec(it.first, tol);
                    rs[iTri].ed(ie).neighborhood() = it.second;
                }
            }
        }

        YinSet<T> meet(const YinSet<T> &y2, Real tol = TOL)
        {
            //Intersection Triangle.
            TriangleIntersection<T> intersectOp;
            std::vector<Triangle<T, 3>> inputA, inputB;
            collapse(inputA, 1, tol);
            y2.collapse(inputB, 2, tol);
            intersectop(inputA, inputB, tol);

            // Triangulation
            Triangulation<T> triangulateOp;
            triangulateOp(inputA, inputB, tol);

            // RemoveOverlap
            RemoveOverlap<T> removeOverlapOp;
            removeOverlapOp.TriangulateA = triangulateOp.TriangulateA;
            removeOverlapOp.TriangulateB = triangulateOp.TriangulateB;
            removeOverlapOp.vecTriA = triangulateOp.vecTriA;
            removeOverlapOp.vecTriB = triangulateOp.vecTriB;
            removeOverlapOp.resultA = triangulateOp.resultA;
            removeOverlapOp.resultB = triangulateOp.resultB;
            removeOverlapOp();

            // PrePast
            PrePast<T> prePastOpA, prePastOpB;
            prePastOpA(removeOverlapOp.vecTriA, 1, tol);
            prePastOpB(removeOverlapOp.vecTriB, 2, tol);

            // Locate SurfacePatch.
            Locate<T> locateOp;
            locateOp(inputA, inputB,
                     removeOverlapOp.vecTriA, removeOverlapOp.vecTriB,
                     prePastOpA.vecSP, prePastOpB.vecSP, tol);
        }

        void BuildHasse() const;

        ~YinSet();
    };

} // namespace YSB

#endif // !YINSET_H