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
        explicit YinSet(const std::vector<GluingCompactSurface<T>> &vf, Real tol = TOL)
            : vecFace(vf)
        {
            BuildHasse(tol);
        }

        void collapse(std::vector<Triangle<T, 3>> &rs, int idYinset, Real tol = TOL) const
        {
            SegmentCompare segcmp(tol);

            // std::map<Segment<T, 3>, std::vector<std::pair<int, int>>> segs(segcmp);
            int idFace = 0;
            for (auto &&iFace : vecFace)
            {
                iFace.collapse(rs, idYinset, idFace, tol);
                ++idFace;
            }

            // for (auto &&it : segs)
            // {
            //     for (auto &&iTri : it.second)
            //     {
            //         int ie = rs[iTri].edgeVec(it.first, tol);
            //         rs[iTri].ed(ie).neighborhood() = it.second;
            //     }
            // }
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
            removeOverlapOp(triangulateOp.TriangulateA,
                            triangulateOp.TriangulateB,
                            triangulateOp.vecTriA,
                            triangulateOp.vecTriB,
                            triangulateOp.resultA,
                            triangulateOp.resultB, tol);

            // PrePast
            PrePast<T> prePastOpA, prePastOpB;
            prePastOpA(triangulateOp.vecTriA, 1, tol);
            prePastOpB(triangulateOp.vecTriB, 2, tol);

            // Locate SurfacePatch.
            Locate<T> locateOp;
            locateOp(inputA, inputB,
                     triangulateOp.vecTriA, triangulateOp.vecTriB,
                     prePastOpA.vecSP, prePastOpB.vecSP, tol);

            // Past SurfacePatch to GluingCompactSurface.
            Past<T> pastOp;
            std::vector<SurfacePatch<T>> vecF;
            pastOp.combine(prePastOpA.vecSP, prePastOpB.vecSP,
                           triangulateOp.vecTriA, triangulateOp.vecTriB,
                           vecF);
            pastOp(vecF, triangulateOp.vecTriA, triangulateOp.vecTriB, tol);

            // Yinset Constructor
            return YinSet<T>(pastOp.vecGCS, tol);
        }

        void BuildHasse(Real tol = TOL) const;

        ~YinSet();
    };
} // namespace YSB

#endif // !YINSET_H