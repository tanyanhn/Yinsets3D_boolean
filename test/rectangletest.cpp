#include "../catch.hpp"
#include "../src/Rectangle.h"

using namespace YSB;
using namespace std;

TEST_CASE("Rectangle1", "[rect1]") {
  Real r1[3] = {0.0, 0.0, 0.0}, r2[3] = {1.0, 0.0, 0.0},
       r3[3] = {1.0, 1.0, 0.0}, r4[3] = {0.0, 1.0, 0.0},
       r5[3] = {0.75, 0.25, 0.0}, r6[3] = {0.25, 0.75, 0.0},
       r7[3] = {0.5, 0.5, 1.0};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6), tp7(r7);
  Rectangle<Real, 3> rect(tp1, tp2, tp3, tp4);

  Point<Real, 3> tripoints1[] = {tp5, tp6, tp7};
  Triangle<Real, 3> tri1(tripoints1);
  vector<Segment<Real, 3>> result;
  REQUIRE(rect.perimeter() == 4.0);
  rect.intersect(tri1, result);

  REQUIRE(result.size() == 2);
}