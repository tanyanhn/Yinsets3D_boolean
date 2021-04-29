#ifndef YINSET_H
#define YINSET_H
#include "GluingCompactSurface.h"
#include "SegmentCompare.h"
#include "TriangleIntersection.h"
#include "Triangulation.h"
#include "RemoveOverlap.h"
#include "ReFactoryBoundary.h"
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
            //BuildHasse(tol);
        }

        std::vector<GluingCompactSurface<T>> &gcss() { return vecFace; }
        const std::vector<GluingCompactSurface<T>> &gcss() const { return vecFace; }

        void collapse(std::vector<Triangle<T, 3>> &rs, int idYinset, Real tol = TOL) 
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

        YinSet<T> complement(const int usingRFB = 0, Real tol = TOL)
        {
            TriangleIntersection<T> intersectOp;
            std::vector<Triangle<T, 3>> inputA, inputB;
            collapse(inputA, 1, tol);
            intersectOp(inputA, inputB, tol);

            // Triangulation
            Triangulation<T> triangulateOp;
            triangulateOp(inputA, inputB,
                          intersectOp.resultA, intersectOp.resultB, tol);
            
            

            // PrePast
            PrePast<T> prePastOp;
            prePastOp(triangulateOp.vecTriA, tol);
            for(auto &&tri : triangulateOp.vecTriA)
            {
                tri.reverse();
            }
            // std::vector<Triangle<T, 3>> *pVecTriA = &triangulateOp.vecTriA;
            // std::vector<Triangle<T, 3>> *pVecTriB = &triangulateOp.vecTriB;
            // std::vector<SurfacePatch<T>> *pVecSPA = &prePastOp.vecSPA;
            // std::vector<SurfacePatch<T>> *pVecSPB = &prePastOp.vecSPB;
            // std::map<std::pair<int, int>,
            //          std::vector<std::pair<int, int>>> *pClipFaces = &prePastOp.ClipFaces;
            // std::map<std::pair<int, int>,
            //          std::vector<std::pair<int, int>>> *pCoClipFaces = &prePastOp.coClipFaces;

            // if (usingRFB == 1)
            // {
            //     ReFactoryBoundary<T> reFactoryBoundaryOp;
            //     reFactoryBoundaryOp(triangulateOp.vecTriA, triangulateOp.vecTriB,
            //                         prePastOp.vecSPA, prePastOp.vecSPB,
            //                         prePastOp.ClipFaces, prePastOp.coClipFaces, tol);
            // }

            // Locate SurfacePatch.

            // Past SurfacePatch to GluingCompactSurface.
            Past<T> pastOp;
            std::vector<SurfacePatch<T>> vecF;
            pastOp.combine(prePastOp.vecSPA, prePastOp.vecSPB,
                           triangulateOp.vecTriA, triangulateOp.vecTriB,
                           vecF);
            pastOp(vecF, triangulateOp.vecTriA, triangulateOp.vecTriB, tol);

            // Yinset Constructor
            return YinSet<T>(pastOp.vecGCS, tol);
        }

        YinSet<T> meet(YinSet<T> &y2, const int usingRFB = 0, Real tol = TOL)
        {
            //Intersection Triangle.
            TriangleIntersection<T> intersectOp;
            std::vector<Triangle<T, 3>> inputA, inputB;
            collapse(inputA, 1, tol);
            y2.collapse(inputB, 2, tol);
            // for(int i=0;i<inputA.size();i++)
            // {
            //     T r1[3] = {-0.525730, -0.850652, 0.000000}, r2[3] = {-0.894426,-0.447216,0.000000}, r3[3] = {-0.688189, -0.525736, -0.499997};
            //     Point<T, 3> tp1(r1), tp2(r2), tp3(r3);
            //     Point<T, 3> tripoints1[] = {tp1, tp2, tp3};
            //     Triangle<Real, 3> tri1(tripoints1);
            //     if(tri1.equal(inputA[i]))
            //     {
            //         int a = i;
            //     }
            // }
            //std::cout<<inputA.size()<<" "<<inputB.size()<<std::endl;
            intersectOp(inputA, inputB, tol);

            // Triangulation
            Triangulation<T> triangulateOp;
            triangulateOp(inputA, inputB,
                          intersectOp.resultA, intersectOp.resultB, tol);
            
            // RemoveOverlap
            RemoveOverlap<T> removeOverlapOp;
            removeOverlapOp(triangulateOp.TriangulateA,
                            triangulateOp.TriangulateB,
                            triangulateOp.vecTriA,
                            triangulateOp.vecTriB,
                            intersectOp.resultA,
                            intersectOp.resultB, tol);

            // PrePast
            PrePast<T> prePastOp;
            prePastOp(triangulateOp.vecTriA, triangulateOp.vecTriB, tol);

            // std::vector<Triangle<T, 3>> *pVecTriA = &triangulateOp.vecTriA;
            // std::vector<Triangle<T, 3>> *pVecTriB = &triangulateOp.vecTriB;
            // std::vector<SurfacePatch<T>> *pVecSPA = &prePastOp.vecSPA;
            // std::vector<SurfacePatch<T>> *pVecSPB = &prePastOp.vecSPB;
            // std::map<std::pair<int, int>,
            //          std::vector<std::pair<int, int>>> *pClipFaces = &prePastOp.ClipFaces;
            // std::map<std::pair<int, int>,
            //          std::vector<std::pair<int, int>>> *pCoClipFaces = &prePastOp.coClipFaces;

            // if (usingRFB == 1)
            // {
            //     ReFactoryBoundary<T> reFactoryBoundaryOp;
            //     reFactoryBoundaryOp(triangulateOp.vecTriA, triangulateOp.vecTriB,
            //                         prePastOp.vecSPA, prePastOp.vecSPB,
            //                         prePastOp.ClipFaces, prePastOp.coClipFaces, tol);
            // }

            // Locate SurfacePatch.
            Locate<T> locateOp;
            locateOp(inputA, inputB, vecFace[0].Type(), y2.vecFace[0].Type(),
                     triangulateOp.vecTriA, triangulateOp.vecTriB,
                     prePastOp.vecSPA, prePastOp.vecSPB, tol);

            // Past SurfacePatch to GluingCompactSurface.
            Past<T> pastOp;
            std::vector<SurfacePatch<T>> vecF;
            pastOp.combine(prePastOp.vecSPA, prePastOp.vecSPB,
                           triangulateOp.vecTriA, triangulateOp.vecTriB,
                           vecF);
            pastOp(vecF, triangulateOp.vecTriA, triangulateOp.vecTriB, tol);

            // Yinset Constructor
            return YinSet<T>(pastOp.vecGCS, tol);
        }

        YinSet<T> join(YinSet<T> &y2, const int usingRFB = 0, Real tol = TOL)
        {
            auto comp1 = complement();
            auto comp2 = y2.complement();
            auto comp3 = comp1.meet(comp2);
            return comp3.complement();
        }

        // void BuildHasse(Real tol = TOL) const;


    };
} // namespace YSB

#endif // !YINSET_H