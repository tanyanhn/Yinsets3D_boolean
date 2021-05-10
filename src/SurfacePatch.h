#ifndef SURFACEPATCH_H
#define SURFACEPATCH_H

#include <map>
#include "SegmentCompare.h"
#include "Triangle.h"

namespace YSB {

template <class T>
struct Locate;

template <class T>
class SurfacePatch {
  friend struct Locate<T>;

 private:
  std::vector<std::pair<int, int>> vecTriangle;
  std::map<Segment<T, 3>, std::vector<std::pair<int, int>>, SegmentCompare>
      boundary;
  bool removed = false;

 public:
  SurfacePatch(
      const std::vector<std::pair<int, int>>& t,
      std::map<Segment<T, 3>, std::vector<std::pair<int, int>>, SegmentCompare>&
          b)
      : vecTriangle(t), boundary(b) {}

  SurfacePatch(const SurfacePatch<T>& SP1, const SurfacePatch<T>& SP2)
      : vecTriangle(SP1.vecTriangle), boundary(SP1.boundary) {
    vecTriangle.insert(vecTriangle.end(), SP2.vecTriangle.begin(),
                       SP2.vecTriangle.end());
    for (auto&& itb : SP2.boundary) {
      auto it = boundary.insert(itb);
      if (it.second == false) {
        ((it.first)->second)
            .insert(((it.first)->second).end(), (itb.second).begin(),
                    (itb.second).end());
      }
    }
  }

  // Accessors
  std::vector<std::pair<int, int>>& tris() { return vecTriangle; }

  const std::vector<std::pair<int, int>>& tris() const { return vecTriangle; }

  std::map<Segment<T, 3>, std::vector<std::pair<int, int>>, SegmentCompare>&
  bound() {
    return boundary;
  }

  const std::
      map<Segment<T, 3>, std::vector<std::pair<int, int>>, SegmentCompare>&
      bound() const {
    return boundary;
  }

  bool& IfRemoved() { return removed; }

  const bool& IfRemoved() const { return removed; }

  ~SurfacePatch() = default;
};

}  // namespace YSB

#endif  // !SURFACEPATCH_H