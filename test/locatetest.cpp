#include <vector>
#include "../catch.hpp"
#include "../src/Locate.h"

using namespace YSB;
using namespace std;

TEST_CASE("Triangle locate", "[tl1]") {
  Real r1[3] = {1.0, 1.0, 0.0}, r2[3] = {0.0, 0.0, 0.0},
       r3[3] = {2.0, 0.0, 0.0}, r4[3] = {1.0, 1.0, 1.0};
  Real r5[3] = {1.0, 1.0, 1.0}, r6[3] = {0.8, 0.7, 0.01},
       r7[3] = {-0.1, 0.0, 0.0};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), p1(r5), p2(r6), p3(r7);
  Point<Real, 3> tripoints1[] = {tp1, tp3, tp2}, tripoints2[] = {tp3, tp4, tp2},
                 tripoints3[] = {tp1, tp4, tp3}, tripoints4[] = {tp1, tp2, tp4};
  Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3),
      tri4(tripoints4);
  vector<Triangle<Real, 3>> tris{tri1, tri2, tri3, tri4};
  Locate<Real> loc;

  REQUIRE(loc(tris, p1) == 0);
  REQUIRE(loc(tris, p2) == 1);
  REQUIRE(loc(tris, p3) < 0);
}
