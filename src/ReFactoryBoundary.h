#ifndef REFACTORYBOUNDARY_H
#define REFACTORYBOUNDARY_H

#include "FindNearTriangle.h"
#include "RemoveOverlap.h"
#include "SurfacePatch.h"

namespace YSB {
template <class T>
struct ReFactoryBoundary {
  void operator()(
      std::vector<Triangle<T, 3>>& vecTriA,
      std::vector<Triangle<T, 3>>& vecTriB,
      std::vector<SurfacePatch<T>>& vecSPA,
      std::vector<SurfacePatch<T>>& vecSPB,
      std::map<std::pair<int, int>, std::set<std::pair<int, int>>>& ClipFaces,
      Real tol = TOL) {
    this->work(vecTriA, vecTriB, vecSPA, vecSPB, ClipFaces, tol);
  }

  void work(
      std::vector<Triangle<T, 3>>& vecTriA,
      std::vector<Triangle<T, 3>>& vecTriB,
      std::vector<SurfacePatch<T>>& vecSPA,
      std::vector<SurfacePatch<T>>& vecSPB,
      std::map<std::pair<int, int>, std::set<std::pair<int, int>>>& ClipFaces,
      Real tol = TOL) {
    SegmentCompare segCmp(tol);
    FindNearTriangle<T> FNTOp;
    std::vector<Triangle<T, 3>>* vecTriar[2] = {&vecTriA, &vecTriB};
    std::vector<SurfacePatch<T>>* vecSPar[2] = {&vecSPA, &vecSPB};
    std::set<std::pair<int, int>> All;
    std::map<std::pair<std::pair<int, int>, std::pair<int, int>>,
             std::pair<int, int>>
        comb;

    for (auto&& itCF : ClipFaces) {
      // All.insert(itCF.first);
      All.insert(itCF.second.begin(), itCF.second.end());
    }

    while (!All.empty()) {
      std::pair<int, int> idVecSP = *All.begin();
      All.erase(All.begin());
      int idYinSet = idVecSP.first;
      int bsize =
          (*(vecSPar[idVecSP.first - 1]))[idVecSP.second].bound().size();
      for (int i = 0; i < bsize; ++i) {
        auto it = std::next(
            (*(vecSPar[idVecSP.first - 1]))[idVecSP.second].bound().begin(), i);
        Segment<T, 3> seg(it->first);
        if (it->second.size() > 1) {
          std::vector<Triangle<T, 3>> triangles;
          std::vector<std::pair<int, int>> Neighbor;
          for (auto idTri : it->second) {
            triangles.push_back((*(vecTriar[idTri.first - 1]))[idTri.second]);
            auto eit = std::remove(seg.neighborhood().begin(),
                                   seg.neighborhood().end(), idTri);
            seg.neighborhood().erase(eit, seg.neighborhood().end());
          }

          // For each SP, boundary remain at most one triangle.
          // And past other triangle by good pair. adjust neighbor.
          int t0 = 0, t1 = 0;
          while (triangles.size() > 1) {
            t1 = FNTOp(triangles[t0], seg, triangles, tol);
            if (t0 == FNTOp(triangles[t1], seg, triangles, tol)) {
              int ie0 = triangles[t0].edgeVec(it->first),
                  ie1 = triangles[t1].edgeVec(it->first);
              Neighbor.clear();
              Neighbor.push_back({idYinSet, triangles[t0].id()});
              Neighbor.push_back({idYinSet, triangles[t1].id()});
              (*(vecTriar[idYinSet - 1]))[triangles[t0].id()]
                  .ed(ie0)
                  .neighborhood() = Neighbor;
              (*(vecTriar[idYinSet - 1]))[triangles[t1].id()]
                  .ed(ie1)
                  .neighborhood() = Neighbor;
              (*(vecTriar[idYinSet - 1]))[triangles[t0].id()]
                  .ed(ie0)
                  .IntersectionSeg() = 0;
              (*(vecTriar[idYinSet - 1]))[triangles[t1].id()]
                  .ed(ie1)
                  .IntersectionSeg() = 0;
              triangles.erase(std::next(triangles.begin(), t0));
              triangles.erase(std::next(triangles.begin(), t1));
            } else
              t0 = t1;
          }

          // deal the last is not pasted triangle.neighbor and boundary or erase
          // the boundary.
          it->second.clear();
          if (triangles.size() == 1) {
            it->second.push_back({idYinSet, triangles[0].id()});
            seg.neighborhood().push_back({idYinSet, triangles[0].id()});
          }

          // Get the boundary also contained by which SP and GCS.
          // And adjust these boundary and triangle.ed.
          auto idOtherTris = seg.neighborhood();

          std::set<std::pair<int, int>> idOtherSPs;
          // idOtherGCSs;
          for (auto&& itTri : idOtherTris) {
            auto idOtherSP = (*(vecTriar[itTri.first - 1]))[itTri.second].inF();
            idOtherSPs.insert(idOtherSP);

            // Pull otherTris along erased segment.
            int ie =
                (*(vecTriar[itTri.first - 1]))[itTri.second].edgeVec(seg, tol);
            (*(vecTriar[itTri.first - 1]))[itTri.second].ed(ie).neighborhood() =
                idOtherTris;
          }

          for (auto&& itSP : idOtherSPs) {
            auto it =
                (*(vecSPar[itSP.first - 1]))[itSP.second].bound().find(seg);
            assert(it !=
                   (*(vecSPar[itSP.first - 1]))[itSP.second].bound().end());
            auto se = it->second;
            auto newseg = it->first;
            newseg.neighborhood() = idOtherTris;
            (*(vecSPar[itSP.first - 1]))[itSP.second].bound().erase(it);
            (*(vecSPar[itSP.first - 1]))[itSP.second].bound().insert(
                {newseg, se});
          }

          // while only one GCS contain the seg. erase the intersection
          // And past the GCS' s ClipFaces  SPs cross seg by good pair.
          if (idOtherTris.size() == 2 && triangles.size() != 1) {
            std::vector<Triangle<T, 3>> triangles;
            std::vector<std::pair<int, int>> Neighbor;
            for (auto idTri : idOtherTris) {
              triangles.push_back((*(vecTriar[idTri.first - 1]))[idTri.second]);
            }
            int idOtherYinSet = idOtherTris.begin()->first;

            // Past by good pair.
            int t0 = 0, t1 = 1;
            int ie0 = triangles[t0].edgeVec(seg),
                ie1 = triangles[t1].edgeVec(seg);
            Neighbor.clear();
            Neighbor.push_back({idOtherYinSet, triangles[t0].id()});
            Neighbor.push_back({idOtherYinSet, triangles[t1].id()});
            (*(vecTriar[idOtherYinSet - 1]))[triangles[t0].id()]
                .ed(ie0)
                .neighborhood() = Neighbor;
            (*(vecTriar[idOtherYinSet - 1]))[triangles[t1].id()]
                .ed(ie1)
                .neighborhood() = Neighbor;
            (*(vecTriar[idOtherYinSet - 1]))[triangles[t0].id()]
                .ed(ie0)
                .IntersectionSeg() = 0;
            (*(vecTriar[idOtherYinSet - 1]))[triangles[t1].id()]
                .ed(ie1)
                .IntersectionSeg() = 0;
            // Past SP in the GCS get new SP cross seg by good pair.
            auto idSP =
                std::make_pair(triangles[t0].inF(), triangles[t1].inF());
            if (idSP.first == idSP.second) {
              comb.insert({idSP, idSP.first});
            } else {
              auto itcomp = comb.find(idSP);
              if (itcomp == comb.end()) {
                auto iface = std::make_pair(
                    idSP.first.first, vecSPar[idSP.first.first - 1]->size());
                comb.insert({idSP, iface});
                SurfacePatch<T> newSP(
                    (*(vecSPar[idSP.first.first - 1]))[idSP.first.second],
                    (*(vecSPar[idSP.second.first - 1]))[idSP.second.second]);
                (*vecSPar[idSP.first.first - 1]).push_back(newSP);

                for (auto&& itTri :
                     (*(vecSPar[idSP.first.first - 1]))[idSP.first.second]
                         .tris())
                  (*(vecTriar[itTri.first - 1]))[itTri.second].inF() = iface;
                for (auto&& itTri :
                     (*(vecSPar[idSP.second.first - 1]))[idSP.second.second]
                         .tris())
                  (*(vecTriar[itTri.first - 1]))[itTri.second].inF() = iface;

                (*(vecSPar[idSP.first.first - 1]))[idSP.first.second]
                    .IfRemoved() = true;
                (*(vecSPar[idSP.second.first - 1]))[idSP.second.second]
                    .IfRemoved() = true;

                All.insert(comb[idSP]);
              }
            }

            (*(vecSPar[idSP.first.first - 1]))[comb[idSP].second].bound().erase(
                seg);
          }
        }
      }

      for (auto it = (*(vecSPar[idVecSP.first - 1]))[idVecSP.second]
                         .bound()
                         .begin(),
                next = it;
           it != (*(vecSPar[idVecSP.first - 1]))[idVecSP.second].bound().end();
           it = next) {
        next = std::next(it);
        if ((it->second).size() == 0)
          (*(vecSPar[idVecSP.first - 1]))[idVecSP.second].bound().erase(it);
      }
    }
    // }
  }
};

}  // namespace YSB

#endif  // !REFACTORYBOUNDARY_H