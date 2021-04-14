#include "../catch.hpp"
#include "../src/FindNearTriangle.h"
#include <vector>

using namespace YSB;
using namespace std;

TEST_CASE("Fine near triangle","[fnt1]"){
  Real r1[3] = {1.0, 0.0, 0.0}, r2[3] = {-1.0, 0.0, 0.0}, r3[3] = {0.0, -2.0, 1.0}, r4[3] = {0.0, 2.0, 2.0};
  Real r5[3] = {0.0, 2.0, 0.0}, r6[3] = {0.0, 2.0, -1.0}, r7[3] = {0.0, 2.0, -3.0};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6), tp7(r7);
  Segment<Real, 3> edge(tp1, tp2);
  Point<Real, 3> tripoints1[] = {tp1, tp2, tp3}, tripoints2[] = {tp2, tp1, tp4}, tripoints3[] = {tp2, tp1, tp5}, tripoints4[] = {tp2, tp1, tp6}, tripoints5[] = {tp1, tp2, tp7};
  Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3), tri4(tripoints4), tri5(tripoints5);
  vector<Triangle<Real, 3>> triA{tri1, tri2, tri3, tri4, tri5}, triB;
  FindNearTriangle<Real> fnt;
  fnt.triangleA = triA;
  fnt.triangleB = triB;
  edge.addneighbor(make_pair(1, 0));
  edge.addneighbor(make_pair(1, 1));
  edge.addneighbor(make_pair(1, 2));
  edge.addneighbor(make_pair(1, 3));
  edge.addneighbor(make_pair(1, 4));
  auto res = fnt(tri1, edge, triA, triB);
  REQUIRE(tri4.equal(triA[res.second]));
}


