#include <vector>
#include "../catch.hpp"
#include "../src/Past.h"
#include "../src/PrePast.h"

using namespace YSB;
using namespace std;

TEST_CASE("Paste", "[pa1]") {
  Real r1[3] = {0.0, -1.0, 0.0}, r2[3] = {1.0, 0.0, 0.0},
       r3[3] = {-1.0, 0.0, 0.0}, r4[3] = {0.0, 1.0, 0.0};
  Real r5[3] = {0.0, 0.0, 3.0};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5);
  Point<Real, 3> tripoints1[] = {tp1, tp2, tp5}, tripoints2[] = {tp2, tp4, tp5},
                 tripoints3[] = {tp2, tp1, tp3}, tripoints4[] = {tp1, tp5, tp3},
                 tripoints5[] = {tp5, tp4, tp3}, tripoints6[] = {tp2, tp3, tp4};
  Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3),
      tri4(tripoints4), tri5(tripoints5), tri6(tripoints6);
  vector<Triangle<Real, 3>> triA{tri1, tri2, tri3, tri4, tri5, tri6};
  GluingCompactSurface<Real> gcs(triA);

  REQUIRE(gcs.Type() == 1);
}
