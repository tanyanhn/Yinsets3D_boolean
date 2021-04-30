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
#ifdef _UsBoost
#include <boost/timer/timer.hpp>
#endif // _UsBoost

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
            ensureFace(tol);
            //BuildHasse(tol);
        }

        std::vector<GluingCompactSurface<T>> &
        gcss()
        {
            return vecFace;
        }
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

        void ensureFace(Real tol = TOL)
        {
            TriangleIntersection<T> intersectOp;
            std::vector<Triangle<T, 3>> inputA, inputB;
            collapse(inputA, 1, tol);
            intersectOp(inputA, inputB, tol);
            for (auto &&tri : intersectOp.resultA)
            {
                // for (auto &&seg : tri.second.first)
                for (auto &&seg : tri.first)
                {
                    if (seg.neighborhood().size() < 2)
                    {
                        std::cout << "Contain face not close.";
                        abort();
                    }
                }
            }
        }

        YinSet<T> complement(const int usingRFB = 0, Real tol = TOL)
        {

            TriangleIntersection<T> intersectOp;
            std::vector<Triangle<T, 3>> inputA, inputB;
            collapse(inputA, 1, tol);
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "complement::TriangleIntersection cpu time : ";
#endif // _UsBoost

                intersectOp(inputA, inputB, tol);
            }

            // Triangulation
            Triangulation<T> triangulateOp;
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "complement::Triangulation cpu time : ";
#endif // _UsBoost

                triangulateOp(inputA, inputB,
                              intersectOp.resultA, intersectOp.resultB, tol);
            }

            // PrePast
            PrePast<T> prePastOp;
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "complement::prePast cpu time : ";
#endif // _UsBoost

                prePastOp(triangulateOp.vecTriA, tol);
            }

            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "complement::reverse cpu time : ";
#endif // _UsBoost

                for (auto &&tri : triangulateOp.vecTriA)
                {
                    tri.reverse();
                }
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
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "complement::Past cpu time : ";
#endif // _UsBoost

                pastOp.combine(prePastOp.vecSPA, prePastOp.vecSPB,
                               triangulateOp.vecTriA, triangulateOp.vecTriB,
                               vecF);
                pastOp(vecF, triangulateOp.vecTriA, triangulateOp.vecTriB, tol);
            }

            // Yinset Constructor
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "complement::YinSet() cpu time : ";
#endif // _UsBoost

                return YinSet<T>(pastOp.vecGCS, tol);
            }
        }

        YinSet<T> meet(YinSet<T> &y2, const int usingRFB = 0, Real tol = TOL)
        {
            //Intersection Triangle.
            TriangleIntersection<T> intersectOp;
            std::vector<Triangle<T, 3>> inputA, inputB;
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "meet::collapse cpu time : ";
#endif // _UsBoost

                collapse(inputA, 1, tol);
                y2.collapse(inputB, 2, tol);
            }

            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "meet::intersect cpu time : ";
#endif // _UsBoost

                intersectOp(inputA, inputB, tol);
            }

            // Triangulation
            Triangulation<T> triangulateOp;
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "meet::triangulate cpu time : ";
#endif // _UsBoost

                triangulateOp(inputA, inputB,
                              intersectOp.resultA, intersectOp.resultB, tol);
            }

            // RemoveOverlap
            RemoveOverlap<T> removeOverlapOp;
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "meet::removeOverlap cpu time : ";
#endif // _UsBoost

                removeOverlapOp(triangulateOp.TriangulateA,
                                triangulateOp.TriangulateB,
                                triangulateOp.vecTriA,
                                triangulateOp.vecTriB,
                                intersectOp.resultA,
                                intersectOp.resultB, tol);
            }

            // PrePast
            PrePast<T> prePastOp;
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "meet::prePast cpu time : ";
#endif // _UsBoost

                prePastOp(triangulateOp.vecTriA, triangulateOp.vecTriB, tol);
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
            // {
            //     boost::timer::auto_cpu_timer t;
            //     std::cout << std::endl
            //               << "meet::reFactoryBoundary cpu time : ";
            //     reFactoryBoundaryOp(triangulateOp.vecTriA, triangulateOp.vecTriB,
            //                         prePastOp.vecSPA, prePastOp.vecSPB,
            //                         prePastOp.ClipFaces, prePastOp.coClipFaces, tol);
            // }
            // }

            // Locate SurfacePatch.
            Locate<T> locateOp;
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "meet::locate cpu time : ";
#endif // _UsBoost

                locateOp(inputA, inputB, vecFace[0].Type(), y2.vecFace[0].Type(),
                         triangulateOp.vecTriA, triangulateOp.vecTriB,
                         prePastOp.vecSPA, prePastOp.vecSPB, tol);
            }

            // Past SurfacePatch to GluingCompactSurface.
            Past<T> pastOp;
            std::vector<SurfacePatch<T>> vecF;
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "meet::Past cpu time : ";
#endif // _UsBoost

                pastOp.combine(prePastOp.vecSPA, prePastOp.vecSPB,
                               triangulateOp.vecTriA, triangulateOp.vecTriB,
                               vecF);
                pastOp(vecF, triangulateOp.vecTriA, triangulateOp.vecTriB, tol);
            }

            // Yinset Constructor
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "meet::YinSet() cpu time : ";
#endif // _UsBoost

                return YinSet<T>(pastOp.vecGCS, tol);
            }
        }

        YinSet<T> join(YinSet<T> &y2, const int usingRFB = 0, Real tol = TOL)
        {
            auto comp1 = complement();
            auto comp2 = y2.complement();
            auto comp3 = comp1.meet(comp2);
            return comp3.complement();
        }

        // void buildHasse(Real tol)
        // {
        //     // step 1 : construct the inclusion matrix
        //     const int numFace = vecFace.size();
        //     Tensor<int, 2> mat(Vec<int, 2>{numCurves, numCurves});
        //     mat = 0;
        //     std::vector<Vec<Real, 2>> somePoints(numCurves);
        //     std::vector<Interval<2>> boxes(numCurves);
        //     std::vector<bool> boundedness(numCurves);
        //     std::vector<std::vector<int>> candidates(numCurves);
        //     for (int i = 0; i < numCurves; ++i)
        //     {
        //         const auto &polys = segmentedCurves[i].getPolys();
        //         const auto &knots = segmentedCurves[i].getKnots();
        //         somePoints[i] = polys[0]((knots[0] + knots[1]) / 2);
        //         boxes[i] = boundingBox(segmentedCurves[i]);
        //         boundedness[i] = ::isBounded(segmentedCurves[i], tol);
        //     }
        //     for (int i = 0; i < numCurves - 1; ++i)
        //     {
        //         for (int j = i + 1; j < numCurves; ++j)
        //         {
        //             if (boxes[i].contain(boxes[j], tol))
        //                 candidates[i].push_back(j);
        //             if (boxes[j].contain(boxes[i], tol))
        //                 candidates[j].push_back(i);
        //         }
        //     }
        //     //
        //     PointsLocater locater(tol);
        //     for (int i = 0; i < numCurves; ++i)
        //     {
        //         std::vector<Vec<Real, 2>> queries;
        //         for (int k : candidates[i])
        //             queries.push_back(somePoints[k]);
        //         auto loc = locater(collapseToSeg(segmentedCurves[i]), queries, boundedness[i]);
        //         for (std::size_t j = 0; j < candidates[i].size(); ++j)
        //         {
        //             assert(loc[j] != 0);
        //             if (boundedness[i] == (loc[j] == 1))
        //             {
        //                 mat(i, candidates[i][j]) = 1;
        //                 mat(candidates[i][j], i) = -1;
        //             }
        //         }
        //     }
        //     // step 2 : obtain the Hasse diagram from the inclusion matrix
        //     diagram.resize(numCurves + 1);
        //     diagram[numCurves].depth = -2;
        //     diagram[numCurves].parent = -1;
        //     std::vector<int> numAnc(numCurves); // number of ancestors
        //     std::vector<int> parent(numCurves);
        //     for (int i = 0; i < numCurves; ++i)
        //     {
        //         numAnc[i] = std::count(&mat(0, i), &mat(0, i + 1), 1);
        //         if (numAnc[i] == 0)
        //         {
        //             parent[i] = numCurves;
        //             diagram[numCurves].depth = (boundedness[i]) ? (-1) : (0);
        //         }
        //     }
        //     assert(diagram[numCurves].depth != -2);
        //     // topological sort
        //     int numRemain = numCurves;
        //     int numPositive = 0;
        //     while (numRemain--)
        //     {
        //         int j = std::find(numAnc.cbegin(), numAnc.cend(), 0) - numAnc.cbegin();
        //         numAnc[j] = -1; // so that j will never be selected again
        //         diagram[j].parent = parent[j];
        //         diagram[j].depth = diagram[parent[j]].depth + 1;
        //         if (diagram[j].depth % 2 == 0)
        //             ++numPositive;
        //         diagram[parent[j]].children.push_back(j);
        //         // handle the children
        //         for (int k = 0; k < numCurves; ++k)
        //         {
        //             if (mat(j, k) == 1)
        //             {
        //                 parent[k] = j;
        //                 --numAnc[k];
        //             }
        //         }
        //     }
        //     // step 3 : calculate the Betti nubmers.
        //     if (diagram[numCurves].depth == -1)
        //     {
        //         // this Yin set is bounded.
        //         bettiNumbers[0] = numPositive;
        //         bettiNumbers[1] = numCurves - numPositive;
        //     }
        //     else
        //     {
        //         // this Yin set is unbounded.
        //         bettiNumbers[0] = numPositive + 1;
        //         bettiNumbers[1] = numCurves - numPositive;
        //     }
        // }

        // void BuildHasse(Real tol = TOL) const;
    };
} // namespace YSB

#endif // !YINSET_H