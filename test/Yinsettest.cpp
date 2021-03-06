#include <vector>
#include "../catch.hpp"
#include "../src/YinSet.h"
#include "exportdata.h"

using namespace YSB;
using namespace std;

TEST_CASE("Paste", "[pa1]") {
  Real r1[3] = {0.0, -0.5, 0.0}, r2[3] = {1.0, 1.0, 0.0},
       r3[3] = {-1.0, 1.0, 0.0}, r4[3] = {0.0, 1.0, 1.0};
  Real r5[3] = {0.0, 0.5, 0.0}, r6[3] = {1.0, -1.0, 0.0},
       r7[3] = {-1.0, -1.0, 0.0}, r8[3] = {0.0, -1.0, 1.0};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6), tp7(r7),
      tp8(r8);
  Point<Real, 3> tripoints1[] = {tp1, tp2, tp4}, tripoints2[] = {tp2, tp3, tp4},
                 tripoints3[] = {tp1, tp4, tp3}, tripoints4[] = {tp2, tp1, tp3},
                 tripoints5[] = {tp5, tp6, tp7}, tripoints6[] = {tp5, tp7, tp8},
                 tripoints7[] = {tp6, tp8, tp7}, tripoints8[] = {tp5, tp8, tp6};
  Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3),
      tri4(tripoints4), tri5(tripoints5), tri6(tripoints6), tri7(tripoints7),
      tri8(tripoints8);
  vector<Triangle<Real, 3>> triA{tri1, tri2, tri3, tri4},
      triB{tri5, tri6, tri7, tri8};

  GluingCompactSurface<Real> gcs1(triA), gcs2(triB);
  vector<GluingCompactSurface<Real>> vecgcs1{gcs1}, vecgcs2{gcs2};
  YinSet<Real> y1(vecgcs1, 1), y2(vecgcs2, 1);
  auto res = y1.meet(y2);
  exportdata("tetrahedronmeet", res, "../res/tetrahedronmeet/");
  REQUIRE(res.gcss().size() == 1);
  REQUIRE(res.gcss()[0].tris().size() == 6);

  auto res2 = y1.join(y2);
  exportdata("tetrahedronjoin", res2, "../res/tetrahedronjoin");

  REQUIRE(1 == 1);
}
