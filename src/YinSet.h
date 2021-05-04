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
#include "Core/Tensor.h"
#include "Core/Interval.h"
#include <sstream>
#include <iomanip>
#include <fstream>
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
        std::vector<Node> diagram;
        int bettiNumbers[2];

    public:
        explicit YinSet(const std::vector<GluingCompactSurface<T>> &vf, Real tol = TOL)
            : vecFace(vf)
        {
            // ensureFace(tol);
            buildHasse(tol);
        }

        explicit YinSet(const std::string &s);

        /// Return if the Yin set is bounded, based on the Hasse diagram.
        int isBounded() const { return diagram.back().depth == -1; }

        /// Get the orientation of the k-th Jordan curve, based on the Hasse diagram.
        int getOrientation(int k) const { return (diagram[k].depth % 2 == 0) ? (1) : (-1); }

        /// Return the Betti numbers.
        int getBettiNumber(int rank) const { return bettiNumbers[rank]; }

        /// Return a tabular version of the Hasse diagram.
        std::string getHasseString() const
        {
            assert(!vecFace.empty());
            const int w = 8;
            std::ostringstream oss;
            oss << std::left << std::setw(w) << " ";
            oss << std::left << std::setw(w) << "Parent";
            oss << std::left << std::setw(w) << "Depth";
            oss << std::left << std::setw(w) << "Orient";
            oss << std::left << "Children"
                << "\n";

            for (std::size_t i = 0; i < diagram.size(); ++i)
            {
                oss << std::left << std::setw(w) << i;
                oss << std::left << std::setw(w) << diagram[i].parent;
                oss << std::left << std::setw(w) << diagram[i].depth;
                if (i < vecFace.size())
                {
                    oss << std::left << std::setw(w) << getOrientation(i);
                }
                else
                {
                    oss << std::left << std::setw(w) << " ";
                }
                oss << "{";
                const std::vector<int> &children = diagram[i].children;
                for (int j : children)
                    oss << j << ",";
                oss << "}"
                    << "\n";
            }
            return oss.str();
        }

        std::vector<GluingCompactSurface<T>> &gcss() { return vecFace; }
        const std::vector<GluingCompactSurface<T>> &gcss() const { return vecFace; }

        void collapse(std::vector<Triangle<T, 3>> &rs, int idYinset, Real tol = TOL) const
        {
            SegmentCompare segcmp(tol);

            int idFace = 0;
            for (auto &&iFace : vecFace)
            {
                iFace.collapse(rs, idYinset, idFace, tol);
                ++idFace;
            }
        }

        void ensureFace(Real tol = TOL)
        {
            TriangleIntersection<T> intersectOp;
            std::vector<Triangle<T, 3>> inputA, inputB;
            collapse(inputA, 1, tol);
            intersectOp(inputA, inputB, tol);
            SegmentCompare cmp(tol);
            int i = 0;
            for (auto &&tri : intersectOp.resultA)
            {
                int ie[] = {1, 1, 1};
                // for (auto &&seg : tri.second.first)
                for (auto &&seg : tri.first)
                {
                    for (auto i = 0; i < 3; ++i)
                    {
                        if (cmp.compare(seg, inputA[i].ed(i)) == 0)
                        {
                            ie[i] = 0;
                        }
                    }
                    if (seg.neighborhood().size() < 2)
                    {
                        std::cout << "Contain face not close.";
                        abort();
                    }
                }

                if (ie[0] || ie[1] || ie[2])
                {
                    std::cout << "Contain face not close.";
                    abort();
                }
                i++;
            }
        }

        YinSet<T> complement(const int usingRFB = 0, Real tol = TOL) const
        {

            TriangleIntersection<T> intersectOp;
            std::vector<Triangle<T, 3>> inputA, inputB;

            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "complement::collapse cpu time : ";
#endif // _UsBoost

                collapse(inputA, 1, tol);
            }

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

            if (usingRFB == 1)
            {
                ReFactoryBoundary<T> reFactoryBoundaryOp;
                {
#ifdef _UsBoost
                    boost::timer::auto_cpu_timer t;
                    std::cout << std::endl
                              << "complement::reFactoryBoundary cpu time : ";
#endif // _UsBoost
                    reFactoryBoundaryOp(triangulateOp.vecTriA, triangulateOp.vecTriB,
                                        prePastOp.vecSPA, prePastOp.vecSPB,
                                        prePastOp.ClipFaces, prePastOp.coClipFaces, tol);
                }
            }

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

        YinSet<T> meet(const YinSet<T> &y2, const int usingRFB = 0, Real tol = TOL) const
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

            if (usingRFB == 1)
            {
                ReFactoryBoundary<T> reFactoryBoundaryOp;
                {
#ifdef _UsBoost
                    boost::timer::auto_cpu_timer t;
                    std::cout << std::endl
                              << "meet::reFactoryBoundary cpu time : ";
#endif // _UsBoost
                    reFactoryBoundaryOp(triangulateOp.vecTriA, triangulateOp.vecTriB,
                                        prePastOp.vecSPA, prePastOp.vecSPB,
                                        prePastOp.ClipFaces, prePastOp.coClipFaces, tol);
                }
            }

            // Locate SurfacePatch.
            Locate<T> locateOp;
            {
#ifdef _UsBoost
                boost::timer::auto_cpu_timer t;
                std::cout << std::endl
                          << "meet::locate cpu time : ";
#endif // _UsBoost

                locateOp(inputA, inputB, isBounded(), y2.isBounded(),
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

        YinSet<T> join(YinSet<T> &y2, const int usingRFB = 0, Real tol = TOL) const
        {
            auto comp1 = complement(usingRFB);
            auto comp2 = y2.complement(usingRFB);
            auto comp3 = comp1.meet(comp2, usingRFB);
            return comp3.complement(usingRFB);
        }

        void buildHasse(Real tol)
        {
            // step 1 : construct the inclusion matrix
            const int numFaces = vecFace.size();
            Tensor<int, 2> mat(Box<2>(Vec<int, 2>(0), Vec<int, 2>{numFaces, numFaces} - 1));
            mat = 0;
            std::vector<Vec<Real, 3>> somePoints(numFaces);
            std::vector<Interval<3>> boxes(numFaces);
            std::vector<bool> boundedness(numFaces);
            std::vector<std::vector<int>> candidates(numFaces);
            Real co[3] = {1 / 3.0, 1 / 3.0, 1 / 3.0};
            Point<Real, 3> zero(0.0);
            for (int i = 0; i < numFaces; ++i)
            {
                somePoints[i] = vecFace[i].tris()[0].barycentric(co) - zero;
                boxes[i] = boundingBox(vecFace[i].tris());
                boundedness[i] = (vecFace[i].Type() == FacType::Inner);
            }
            for (int i = 0; i < numFaces - 1; ++i)
            {
                for (int j = i + 1; j < numFaces; ++j)
                {
                    if (boxes[i].contain(boxes[j], tol))
                        candidates[i].push_back(j);
                    if (boxes[j].contain(boxes[i], tol))
                        candidates[j].push_back(i);
                }
            }
            //
            Locate<T> locater;
            for (int i = 0; i < numFaces; ++i)
            {
                std::vector<Point<Real, 3>> queries;
                for (int k : candidates[i])
                    queries.push_back(zero + somePoints[k]);
                auto loc = locater(vecFace[i].tris(), queries, boundedness[i]);
                for (std::size_t j = 0; j < candidates[i].size(); ++j)
                {
                    assert(loc[j] != 0);
                    if (boundedness[i] == (loc[j] == 1))
                    {
                        mat(i, candidates[i][j]) = 1;
                        mat(candidates[i][j], i) = -1;
                    }
                }
            }
            // step 2 : obtain the Hasse diagram from the inclusion matrix
            diagram.resize(numFaces + 1);
            diagram[numFaces].depth = -2;
            diagram[numFaces].parent = -1;
            std::vector<int> numAnc(numFaces); // number of ancestors
            std::vector<int> parent(numFaces);
            for (int i = 0; i < numFaces; ++i)
            {
                numAnc[i] = std::count(&mat(0, i), &mat(0, i + 1), 1);
                if (numAnc[i] == 0)
                {
                    parent[i] = numFaces;
                    diagram[numFaces].depth = (boundedness[i]) ? (-1) : (0);
                }
            }
            assert(diagram[numFaces].depth != -2);
            // topological sort
            int numRemain = numFaces;
            int numPositive = 0;
            while (numRemain--)
            {
                int j = std::find(numAnc.cbegin(), numAnc.cend(), 0) - numAnc.cbegin();
                numAnc[j] = -1; // so that j will never be selected again
                diagram[j].parent = parent[j];
                diagram[j].depth = diagram[parent[j]].depth + 1;
                if (diagram[j].depth % 2 == 0)
                    ++numPositive;
                diagram[parent[j]].children.push_back(j);
                // handle the children
                for (int k = 0; k < numFaces; ++k)
                {
                    if (mat(j, k) == 1)
                    {
                        parent[k] = j;
                        --numAnc[k];
                    }
                }
            }
            // step 3 : calculate the Betti nubmers.
            if (diagram[numFaces].depth == -1)
            {
                // this Yin set is bounded.
                bettiNumbers[0] = numPositive;
                bettiNumbers[1] = numFaces - numPositive;
            }
            else
            {
                // this Yin set is unbounded.
                bettiNumbers[0] = numPositive + 1;
                bettiNumbers[1] = numFaces - numPositive;
            }
        }

        void objOutput(std::string s, std::string folder)
        {
            exportdata_inner(s, *this, folder);
        }
    };

    YinSet<Real> objInput(const std::string &s);
    void exportdata_inner(std::string s, YinSet<Real> &y, std::string folder);

    template <>
    inline YinSet<Real>::YinSet(const std::string &s)
        : YinSet<Real>(objInput(s)) {}

} // namespace YSB

#endif // !YINSET_H