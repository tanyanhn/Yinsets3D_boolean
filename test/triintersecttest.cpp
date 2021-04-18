#include "../catch.hpp"
#include "../src/TriangleIntersection.h"
#include <vector>

using namespace YSB;
using namespace std;

TEST_CASE("Triangle Intersection1", "[tin1]")
{
    Real r1[3] = {1.0, 0.0, 0.0}, r2[3] = {-1.0, 0.0, 0.0}, r3[3] = {0.0, -2.0, 1.0}, r4[3] = {0.0, 2.0, 2.0};
    Real r5[3] = {0.0, 2.0, 0.0}, r6[3] = {0.0, 2.0, -1.0}, r7[3] = {0.0, 2.0, -3.0};
    Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6), tp7(r7);
    Segment<Real, 3> edge(tp1, tp2);
    Point<Real, 3> tripoints1[] = {tp1, tp2, tp3}, tripoints2[] = {tp2, tp1, tp4}, tripoints3[] = {tp2, tp1, tp5},
                   tripoints4[] = {tp2, tp1, tp6}, tripoints5[] = {tp1, tp2, tp7};
    Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3), tri4(tripoints4), tri5(tripoints5);
    vector<Triangle<Real, 3>> triB{tri2, tri3, tri4, tri5}, triA{tri1};
    TriangleIntersection<Real> tin;
    tin(triA, triB);
    REQUIRE(tin.resultA[0].second.size() == 1);
    REQUIRE(tin.resultA[0].first.size() == 9);
    REQUIRE(tin.resultB[0].first.size() == 4);
    REQUIRE(tin.resultB[0].second.size() == 0);
    REQUIRE(tin.resultB[1].first.size() == 4);
    REQUIRE(tin.resultB[2].first.size() == 9);
    REQUIRE(tin.resultB[2].second.size() == 1);
    REQUIRE(tin.resultB[3].first.size() == 4);
}

TEST_CASE("Triangle Intersection2", "[tin2]")
{
    Real r1[3] = {1.0, 0.0, 0.0}, r2[3] = {-1.0, 0.0, 0.0}, r3[3] = {0.0, -2.0, 1.0}, r4[3] = {0.0, -3.0, 20.0};
    Real r5[3] = {0.0, -1.0, 20.0}, r6[3] = {0.0, 2.0, -1.0}, r7[3] = {0.0, 2.0, 3.0}, r8[3] = {1.0, -1.0, 0.5}, r9[3] = {-1.0, -1.0, 0.5};
    Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6), tp7(r7), tp8(r8), tp9(r9);
    Point<Real, 3> tripoints1[] = {tp1, tp2, tp3}, tripoints2[] = {tp3, tp4, tp5}, tripoints3[] = {tp3, tp6, tp7},
                   tripoints4[] = {tp8, tp9, tp6};
    Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3), tri4(tripoints4);
    vector<Triangle<Real, 3>> triB{tri2, tri3, tri4}, triA{tri1};
    TriangleIntersection<Real> tin;
    tin(triA, triB);
    REQUIRE(tin.resultA[0].second.size() == 1);
    REQUIRE(tin.resultA[0].first.size() == 8);
    REQUIRE(tin.resultB[0].first.size() == 5);
    REQUIRE(tin.resultB[0].second.size() == 1);
    REQUIRE(tin.resultB[1].first.size() == 6);
    REQUIRE(tin.resultB[2].first.size() == 7);
    REQUIRE(tin.resultB[2].second.size() == 1);
}
