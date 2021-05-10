#ifndef TRIANGLEINTERSECTION_H
#define TRIANGLEINTERSECTION_H

#include <omp.h>
#include <map>
#include "Triangle.h"
#include "TriangleCompare.h"

namespace YSB {
template <class T>
struct TriangleIntersection {
  using intsType = typename Triangle<T, 3>::intsType;

  std::vector<
      std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>>
      resultA;
  std::vector<
      std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>>
      resultB;

  typedef typename std::vector<
      std::pair<std::vector<Segment<T, 3>>, std::vector<std::pair<int, int>>>>*
      ResultPointer;
  void operator()(const std::vector<Triangle<T, 3>>& inputA,
                  const std::vector<Triangle<T, 3>>& inputB,
                  Real tol = TOL,
                  int w = 1);
  void work1(const std::vector<Triangle<T, 3>>& inputA,
             const std::vector<Triangle<T, 3>>& inputB,
             Real tol = TOL);
  void work2(const std::vector<Triangle<T, 3>>& inputA,
             const std::vector<Triangle<T, 3>>& inputB,
             Real tol = TOL);
  void assureworksame(ResultPointer* backup, ResultPointer* result);
};

template <class T>
inline void TriangleIntersection<T>::operator()(
    const std::vector<Triangle<T, 3>>& inputA,
    const std::vector<Triangle<T, 3>>& inputB,
    Real tol,
    int w) {
  if (w == 1)
    work1(inputA, inputB, tol);

  // if (1)
  // {
  //     auto backupA = resultA, backupB = resultB;
  //     resultA.clear();
  //     resultB.clear
  //     work2(inputA, inputB, tol);

  //     std::vector<std::pair<std::vector<Segment<T, 3>>,
  //                           std::vector<std::pair<int, int>>>>
  //         *resultArr[2] = {&resultA, &resultB},
  //         *backupArr[2] = {&backupA, &backupB};
  //     assureworksame(resultArr, backupArr);
  // }
}

template <class T>
void TriangleIntersection<T>::assureworksame(ResultPointer* backup,
                                             ResultPointer* result) {
  SegmentCompare sCmp(TOL);
  std::set<Segment<T, 3>, SegmentCompare> segs(sCmp), BSegs(sCmp);
  std::set<std::pair<int, int>> laps, BLaps;

  size_t num[2] = {result[0]->size(), result[1]->size()};

  for (int k = 0; k < 2; ++k) {
    for (auto i = 0; i < num[k]; ++i) {
      segs.clear();
      BSegs.clear();
      laps.clear();
      BLaps.clear();
      for (auto seg : (*result[k])[i].first)
        segs.insert(seg);
      for (auto lap : (*result[k])[i].second)
        laps.insert(lap);

      for (auto seg : (*backup[k])[i].first) {
        auto it = segs.find(seg);
        assert(it != segs.end() && "result have more seg.");
        BSegs.insert(seg);
      }
      for (auto lap : (*backup[k])[i].second) {
        auto it = laps.find(lap);
        assert(it != laps.end() && "result have more lap.");
        BLaps.insert(lap);
      }

      assert(BSegs.size() == segs.size() && "result miss segs");
      assert(BLaps.size() == laps.size() && "result miss laps");
    }
  }

  // assert(false && "Passed.");
}

template <class T>
inline void TriangleIntersection<T>::work1(
    const std::vector<Triangle<T, 3>>& inputA,
    const std::vector<Triangle<T, 3>>& inputB,
    Real tol) {
  int numA = inputA.size(), numB = inputB.size();
  resultA.resize(numA);
  resultB.resize(numB);
  const std::vector<Triangle<T, 3>>* inputArr[2] = {&inputA, &inputB};
  std::vector<std::pair<std::vector<Segment<T, 3>>,
                        std::vector<std::pair<int, int>>>>* resultArr[2] = {
      &resultA, &resultB};
  PointCompare pCmp(tol);
  TriangleCompare triCmp(tol);
  std::map<Point<T, 3>, std::vector<std::pair<int, int>>, PointCompare> mt(
      pCmp),
      ml(pCmp);
  std::vector<std::pair<int, int>> interTris, interTrisadd;
  std::set<Point<T, 3>, PointCompare> allP(pCmp);
  Point<T, 3> min, max;

  for (int idYinset = 1; idYinset < 3; ++idYinset) {
    for (size_t idA = 0; idA < (*inputArr[idYinset - 1]).size(); ++idA) {
      auto minmax = (*inputArr[idYinset - 1])[idA].minmax(pCmp);

      min = (*inputArr[idYinset - 1])[idA].vert(minmax.first);
      max = (*inputArr[idYinset - 1])[idA].vert(minmax.second);

      allP.insert(min);
      allP.insert(max);

      auto it = mt.find(max);
      if (it == mt.end())
        mt.insert({max, std::vector<std::pair<int, int>>(1, {idYinset, idA})});
      else
        it->second.push_back({idYinset, idA});

      it = ml.find(min);
      if (it == ml.end())
        ml.insert({min, std::vector<std::pair<int, int>>(1, {idYinset, idA})});
      else
        it->second.push_back({idYinset, idA});
    }
  }

  for (auto&& p : allP) {
    for (auto&& idTri : mt[p]) {
      auto eit = std::remove(interTris.begin(), interTris.end(), idTri);
      interTris.erase(eit, interTris.end());
    }

    int presize = interTris.size();
    for (auto&& idTri1 : ml[p]) {
      interTris.push_back(idTri1);
    }

    int interTrisSize = interTris.size();

    for (int i = interTrisSize - 1; i > presize - 1; --i) {
      for (int j = i - 1; j > -1; --j) {
        auto &idTri1 = interTris[i], idTri2 = interTris[j];
        int iA = idTri1.second, iB = idTri2.second;
        int inYinsetA = idTri1.first, inYinsetB = idTri2.first;

        std::vector<Segment<T, 3>> result;

        typename std::vector<Triangle<T, 3>>::const_iterator tri[2] = {
            ((*(inputArr[inYinsetA - 1])).begin() + iA),
            ((*(inputArr[inYinsetB - 1])).begin() + iB)};

        typename std::vector<
            std::pair<std::vector<Segment<T, 3>>,
                      std::vector<std::pair<int, int>>>>::iterator rs[2] = {
            std::next((*(resultArr[inYinsetA - 1])).begin(), iA),
            std::next((*(resultArr[inYinsetB - 1])).begin(), iB)};

        intsType type = (tri[0])->intersect(*(tri[1]), result, tol);

        if (type == intsType::Never) {
          continue;
        } else if (type == intsType::IntsPoint) {
        } else if (type == intsType::IntsSeg) {
        } else if (type == intsType::Overlap) {
          rs[0]->second.push_back({inYinsetB, iB});
          rs[1]->second.push_back({inYinsetA, iA});
        }

        for (auto&& iSeg : result) {
          iSeg.neighborhood().push_back(std::make_pair(inYinsetA, iA));
          iSeg.neighborhood().push_back(std::make_pair(inYinsetB, iB));

          rs[0]->first.push_back(iSeg);
          rs[1]->first.push_back(iSeg);
        }
      }
    }
  }
}

template <class T>
inline void TriangleIntersection<T>::work2(
    const std::vector<Triangle<T, 3>>& inputA,
    const std::vector<Triangle<T, 3>>& inputB,
    Real tol) {
  int numA = inputA.size(), numB = inputB.size();
  resultA.resize(numA);
  resultB.resize(numB);
  const std::vector<Triangle<T, 3>>* inputArr[2] = {&inputA, &inputB};
  std::vector<std::pair<std::vector<Segment<T, 3>>,
                        std::vector<std::pair<int, int>>>>* resultArr[2] = {
      &resultA, &resultB};
  PointCompare pCmp(tol / 100);
  TriangleCompare triCmp(tol / 100);
  std::map<Point<T, 3>, std::vector<std::pair<int, int>>, PointCompare> mt(
      pCmp),
      ml(pCmp);
  std::map<Point<T, 1>, std::vector<std::pair<int, int>>, PointCompare> mtlX(
      pCmp);
  std::map<Point<T, 1>, std::vector<std::pair<int, int>>, PointCompare> mtlY(
      pCmp);
  std::vector<std::pair<int, int>> interTris, interTrisadd;
  std::set<std::pair<int, int>> interTriset, interTrisettmp;
  std::set<Point<T, 3>, PointCompare> allP(pCmp);
  Point<T, 3> min, max;
  Point<T, 1> minX, maxX, minY, maxY;
  std::map<std::pair<int, int>, std::pair<Point<T, 1>, Point<T, 1>>> minmaxX,
      minmaxY;

  for (int idYinset = 1; idYinset < 3; ++idYinset) {
    for (int idA = 0; idA < (*inputArr[idYinset - 1]).size(); ++idA) {
      auto minmax = (*inputArr[idYinset - 1])[idA].minmax(pCmp);

      min = (*inputArr[idYinset - 1])[idA].vert(minmax.first);
      max = (*inputArr[idYinset - 1])[idA].vert(minmax.second);

      allP.insert(min);
      allP.insert(max);

      auto it = mt.find(max);
      if (it == mt.end())
        mt.insert({max, std::vector<std::pair<int, int>>(1, {idYinset, idA})});
      else
        it->second.push_back({idYinset, idA});

      it = ml.find(min);
      if (it == ml.end())
        ml.insert({min, std::vector<std::pair<int, int>>(1, {idYinset, idA})});
      else
        it->second.push_back({idYinset, idA});
    }
  }

  for (auto&& p : allP) {
    for (auto&& idTri : mt[p]) {
      auto eit = std::remove(interTris.begin(), interTris.end(), idTri);
      interTris.erase(eit, interTris.end());

      auto it = mtlX.find(minmaxX[idTri].first);
      assert(it != mtlX.end() && "mtlX should contain idTri in mt[p].");
      auto eTit = std::remove(it->second.begin(), it->second.end(), idTri);
      assert(eTit != it->second.end() && "mtlX.second should contain idTri.");
      if (eTit != it->second.begin())
        it->second.erase(eTit, it->second.end());
      else
        mtlX.erase(it);

      it = mtlX.find(minmaxX[idTri].second);
      // assert(it != mtlX.end() && "mtlX should contain idTri in mt[p].");
      if (it != mtlX.end()) {
        eTit = std::remove(it->second.begin(), it->second.end(), idTri);
        // assert(eTit != it->second.end() && "mtlX.second should contain
        // idTri.");
        if (eTit != it->second.begin())
          it->second.erase(eTit, it->second.end());
        else
          mtlX.erase(it);
      }

      it = mtlY.find(minmaxY[idTri].first);
      assert(it != mtlY.end() && "mtlX should contain idTri in mt[p].");
      eTit = std::remove(it->second.begin(), it->second.end(), idTri);
      assert(eTit != it->second.end() && "mtlX.second should contain idTri.");
      if (eTit != it->second.begin())
        it->second.erase(eTit, it->second.end());
      else
        mtlY.erase(it);

      it = mtlY.find(minmaxY[idTri].second);
      // assert(it != mtlX.end() && "mtlX should contain idTri in mt[p].");
      if (it != mtlY.end()) {
        eTit = std::remove(it->second.begin(), it->second.end(), idTri);
        // assert(eTit != it->second.end() && "mtlX.second should contain
        // idTri.");
        if (eTit != it->second.begin())
          it->second.erase(eTit, it->second.end());
        else
          mtlY.erase(it);
      }

      minmaxX.erase(idTri);
      minmaxY.erase(idTri);
    }

    interTrisadd.clear();
    for (auto&& idTri : ml[p]) {
      interTrisadd.push_back(idTri);
    }

    for (auto&& idTri2 : interTrisadd) {
      interTris.push_back(idTri2);

      auto TriX =
               (*inputArr[idTri2.first - 1])[idTri2.second].project(2).project(
                   1),
           TriY =
               (*inputArr[idTri2.first - 1])[idTri2.second].project(2).project(
                   0);

      auto minmaxXid = TriX.minmax(pCmp), minmaxYid = TriY.minmax(pCmp);

      minX = TriX.vert(minmaxXid.first);
      maxX = TriX.vert(minmaxXid.second);
      minY = TriY.vert(minmaxYid.first);
      maxY = TriY.vert(minmaxYid.second);

      minmaxX[idTri2] = {minX, maxX};
      minmaxY[idTri2] = {minY, maxY};

      auto uppXit =
          mtlX.insert({maxX, std::vector<std::pair<int, int>>(1, idTri2)});
      if (uppXit.second == false)
        uppXit.first->second.push_back(idTri2);

      auto lowXit =
          mtlX.insert({minX, std::vector<std::pair<int, int>>(1, idTri2)});
      if (lowXit.second == false)
        lowXit.first->second.push_back(idTri2);

      auto uppYit =
          mtlY.insert({maxY, std::vector<std::pair<int, int>>(1, idTri2)});
      if (uppYit.second == false)
        uppYit.first->second.push_back(idTri2);

      auto lowYit =
          mtlY.insert({minY, std::vector<std::pair<int, int>>(1, idTri2)});
      if (lowYit.second == false)
        lowYit.first->second.push_back(idTri2);

      interTriset.clear();
      interTrisettmp.clear();
      for (auto it = lowXit.first; it != std::next(uppXit.first); ++it)
        interTrisettmp.insert(it->second.begin(), it->second.end());

      for (auto it = lowYit.first; it != std::next(uppYit.first); ++it) {
        for (auto idTri : it->second) {
          if (interTrisettmp.find(idTri) != interTrisettmp.end()) {
            interTriset.insert(idTri);
          }
        }
      }

      for (auto&& idTri1 : interTriset) {
        if (idTri1 == idTri2)
          continue;

        int iA = idTri1.second, iB = idTri2.second;
        int inYinsetA = idTri1.first, inYinsetB = idTri2.first;

        std::vector<Segment<T, 3>> result;

        typename std::vector<Triangle<T, 3>>::const_iterator tri[2] = {
            ((*(inputArr[inYinsetA - 1])).begin() + iA),
            ((*(inputArr[inYinsetB - 1])).begin() + iB)};

        typename std::vector<
            std::pair<std::vector<Segment<T, 3>>,
                      std::vector<std::pair<int, int>>>>::iterator rs[2] = {
            std::next((*(resultArr[inYinsetA - 1])).begin(), iA),
            std::next((*(resultArr[inYinsetB - 1])).begin(), iB)};

        intsType type = (tri[0])->intersect(*(tri[1]), result, tol);

        if (type == intsType::Never) {
          continue;
        } else if (type == intsType::IntsPoint) {
        } else if (type == intsType::IntsSeg) {
        } else if (type == intsType::Overlap) {
          rs[0]->second.push_back({inYinsetB, iB});
          rs[1]->second.push_back({inYinsetA, iA});
        }

        for (auto&& iSeg : result) {
          iSeg.neighborhood().push_back(std::make_pair(inYinsetA, iA));
          iSeg.neighborhood().push_back(std::make_pair(inYinsetB, iB));

          rs[0]->first.push_back(iSeg);
          rs[1]->first.push_back(iSeg);
        }
      }
    }
  }
}
}  // namespace YSB

#endif  // !TRIANGLEINTERSECTION_H