#ifndef SEGMENTCOMPARE_H
#define SEGMENTCOMPARE_H

#include "Segment.h"

namespace YSB {
class SegmentCompare {
 protected:
  Real tol;

 public:
  explicit SegmentCompare(Real t = TOL);
  ~SegmentCompare() = default;

  // Used for Point compare.
  template <class T1, class T2, int Dim>
  bool operator()(const Segment<T1, Dim>& lhs,
                  const Segment<T2, Dim>& rhs) const {
    return compare(lhs, rhs) == 1;
  }

  template <class T1, class T2, int Dim>
  int compare(const Segment<T1, Dim>& lhs, const Segment<T2, Dim>& rhs) const {
    PointCompare cmp(tol);
    Point<T1, Dim> l[2];
    Point<T2, Dim> r[2];
    for (int i = 0; i < 2; ++i) {
      l[i] = lhs[i];
      r[i] = rhs[i];
    }

    if (cmp(l[0], l[1]))
      std::swap(l[0], l[1]);

    if (cmp(r[0], r[1]))
      std::swap(r[0], r[1]);

    int rs = cmp.compare(l[1], r[1]);
    if (rs == 0) {
      rs = cmp.compare(l[0], r[0]);
    }
    return rs;
  }
};

inline SegmentCompare::SegmentCompare(Real t) {
  tol = t;
}
}  // namespace YSB

#endif  // !SEGMENTCOMPARE_H
