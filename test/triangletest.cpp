#include "../src/Triangle.h"
#include "../catch.hpp"
#include <vector>

using namespace YSB;

TEST_CASE("Triangle locate", "[tl1]") {
  Real r1[3] = {1.0, 1.0, 0.0}, r2[3] = {0.0, 0.0, 0.0},
       r3[3] = {2.0, -2.4, 0.0}, r4[3] = {1.0, 1.0, 0.0},
       r5[3] = {0.5, 0.5, 0.0}, r6[3] = {1.0, 0.0, 0.0},
       r7[3] = {10.0, 1.0, 0.0};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), p1(r4), p2(r5), p3(r6), p4(r7);
  Point<Real, 3> tripoints[] = {tp1, tp2, tp3};
  Triangle<Real, 3> tri(tripoints);
  REQUIRE(tri.locate(p1) == Triangle<Real, 3>::locType::InVertex);
  REQUIRE(tri.locate(p2) == Triangle<Real, 3>::locType::InEdge);
  REQUIRE(tri.locate(p3) == Triangle<Real, 3>::locType::Inter);
  REQUIRE(tri.locate(p4) == Triangle<Real, 3>::locType::Outer);
}

TEST_CASE("3d Triangle intersect line", "[til1]") {
  Real r1[3] = {1.0, 1.0, 0.0}, r2[3] = {0.0, 0.0, 0.0},
       r3[3] = {2.0, -2.4, 0.0}, r4[3] = {1.0, 1.0, 0.0},
       r5[3] = {999.5, 99.5, 1.0}, r6[3] = {1.0, 100.0, 0.0},
       r7[3] = {999.5, 99.5, 1.0}, r8[3] = {1.0, 1.0, 0.0},
       r9[3] = {-0.5, 1.0, 0.0}, r10[3] = {0.4, 0.0, 0.0},
       r11[3] = {2.0, 2.0, 0.0}, r12[3] = {10.0, 10.0, 0.0},
       r13[3] = {0.0, 1.5, 0.0};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), p1(r4), p2(r6), p3(r8), p4(r10),
      p5(r12);
  Vec<Real, 3> d1(r5), d2(r7), d3(r9), d4(r11), d5(r13);
  Point<Real, 3> tripoints[] = {tp1, tp2, tp3};
  Triangle<Real, 3> tri(tripoints);
  Line<Real, 3> l1(p1, d1), l2(p2, d2), l3(p3, d3), l4(p4, d4), l5(p5, d5);
  std::vector<Point<Real, 3>> re1, re2, re3, re4, re5;
  REQUIRE(tri.intersect(l1, re1) == 1);
  REQUIRE(tri.intersect(l2, re2) == 0);
  REQUIRE(tri.intersect(l3, re3) == 1);
  REQUIRE(tri.intersect(l4, re4) == 2);
  REQUIRE(tri.intersect(l5, re5) == 0);
}

TEST_CASE("2d Triangle intersect line", "[2dtil2]") {
  Real r1[2] = {1.0, 1.0}, r2[2] = {0.0, 0.0}, r3[2] = {2.0, -2.5},
       r8[2] = {1.0, 1.0}, r9[2] = {-0.5, 1.0}, r10[2] = {0.4, 0.0},
       r11[2] = {2.0, 2.0}, r12[2] = {10.0, 10.0}, r13[2] = {0.0, 1.5};
  Point<Real, 2> tp1(r1), tp2(r2), tp3(r3), p3(r8), p4(r10), p5(r12);
  Vec<Real, 2> d3(r9), d4(r11), d5(r13);
  Point<Real, 2> tripoints[] = {tp1, tp2, tp3};
  Triangle<Real, 2> tri(tripoints);
  Line<Real, 2> l3(p3, d3), l4(p4, d4), l5(p5, d5);
  std::vector<Point<Real, 2>> re3, re4, re5;
  REQUIRE(tri.intersect(l3, re3) == 1);
  // std::cout<<re3[0]<<std::endl;
  REQUIRE(tri.intersect(l4, re4) == 2);
  // std::cout<<re4[0]<<std::endl;
  // std::cout<<re4[1]<<std::endl;
  REQUIRE(tri.intersect(l5, re5) == 0);
  // std::cout<<std::endl;
}

TEST_CASE("2d Triangle intersect line2", "[2dtil2]") {
  Real r1[2] = {1.0, 1.0}, r2[2] = {0.0, 0.0}, r3[2] = {2.0, 0.0},
       r8[2] = {0.4, 0.2}, r9[2] = {1.0, 1.0};
  Point<Real, 2> tp1(r1), tp2(r2), tp3(r3), p3(r8);
  Vec<Real, 2> d3(r9);
  Point<Real, 2> tripoints[] = {tp1, tp2, tp3};
  Triangle<Real, 2> tri(tripoints);
  Line<Real, 2> l3(p3, d3);
  std::vector<Point<Real, 2>> re3;
  tri.intersect(l3, re3);
  for (int i = 0; i < re3.size(); i++)
    std::cout << re3[i] << std::endl;
  std::cout << std::endl;
  REQUIRE(tri.intersect(l3, re3) == 2);
}

TEST_CASE("2d barycentric", "[2dbc]") {
  Real r1[2] = {1.0, 1.0}, r2[2] = {0.0, 0.0}, r3[2] = {2.0, 0.0},
       r8[2] = {1.0, 1.0}, r9[2] = {1.0, 0.5}, r10[2] = {4.0, 0.0};
  Real ans1[3] = {1.0, 0.0, 0.0}, ans2[3] = {0.5, 0.25, 0.25},
       ans3[3] = {0.0, -1.0, 2.0}, co1[3], co2[3], co3[3];
  Point<Real, 2> tp1(r1), tp2(r2), tp3(r3), p3(r8), p4(r9), p5(r10);
  Point<Real, 2> tripoints[] = {tp1, tp2, tp3};
  Triangle<Real, 2> tri(tripoints);
  Vec<Real, 3> a1(ans1), a2(ans2), a3(ans3);
  tri.barycentric(p3, co1);
  tri.barycentric(p4, co2);
  tri.barycentric(p5, co3);
  Vec<Real, 3> re1(co1), re2(co2), re3(co3);
  REQUIRE(norm(re1 - a1) < TOL);
  REQUIRE(norm(re2 - a2) < TOL);
  REQUIRE(norm(re3 - a3) < TOL);
}

TEST_CASE("3d barycentric", "[3dbc]") {
  Real r1[3] = {1.0, 1.0, 1.0}, r2[3] = {0.5, 2.0, 0.0},
       r3[3] = {-2.0, -2.0, -1.0}, r4[3] = {-0.1, 0.5, 0.0},
       r5[3] = {0.75, 1.5, 0.5}, r6[3] = {-4.5, -6.0, -2.0};
  Real ans1[3] = {0.3, 0.4, 0.3}, ans2[3] = {0.5, 0.5, 0.0},
       ans3[3] = {0.0, -1.0, 2.0}, co1[3], co2[3], co3[3];
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), p1(r4), p2(r5), p3(r6);
  Point<Real, 3> tripoints[] = {tp1, tp2, tp3};
  Triangle<Real, 3> tri(tripoints);
  auto proTri = tri.project(0);
  auto proP1 = p1.project(0), proP2 = p2.project(0), proP3 = p3.project(0);
  proTri.barycentric(proP1, co1);
  proTri.barycentric(proP2, co2);
  proTri.barycentric(proP3, co3);
  Vec<Real, 3> rco1(co1), rco2(co2), rco3(co3), a1(ans1), a2(ans2), a3(ans3);
  REQUIRE(norm(rco1 - a1) < TOL);
  REQUIRE(norm(rco2 - a2) < TOL);
  REQUIRE(norm(rco3 - a3) < TOL);
  Point<Real, 3> rp1 = tri.barycentric(co1), rp2 = tri.barycentric(co2),
                 rp3 = tri.barycentric(co3);
  REQUIRE(norm(rp1 - p1) < TOL);
  REQUIRE(norm(rp2 - p2) < TOL);
  REQUIRE(norm(rp3 - p3) < TOL);
}

TEST_CASE("2d triangle intersect1", "[2dti1]") {
  Real r1[2] = {1.0, 1.0}, r2[2] = {0.0, 0.0}, r3[2] = {2.0, 0.0},
       r4[2] = {1.0, 0.8}, r5[2] = {0.4, 0.2}, r6[2] = {1.6, 0.2},
       r7[2] = {1.0, 2.0}, r8[2] = {1.0, -10.0}, r9[2] = {6.0, 0.0},
       r10[2] = {1.5, 1.0}, r11[2] = {2.5, 0.0}, r12[2] = {1.5, -5.0};
  Point<Real, 2> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6), tp7(r7),
      tp8(r8), tp9(r9), tp10(r10), tp11(r11), tp12(r12);
  Point<Real, 2> tripoints1[] = {tp1, tp2, tp3}, tripoints2[] = {tp4, tp5, tp6},
                 tripoints3[] = {tp7, tp8, tp9},
                 tripoints4[] = {tp10, tp11, tp12};
  Triangle<Real, 2> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3),
      tri4(tripoints4);
  std::vector<Segment<Real, 2>> re1, re2, re3;
  tri1.intersect(tri2, re1);
  tri1.intersect(tri3, re2);
  tri1.intersect(tri4, re3);
  for (int i = 0; i < re1.size(); i++)
    std::cout << re1[i] << std::endl;
  std::cout << std::endl;
  REQUIRE(re1.size() == 3);

  REQUIRE(re2.size() == 4);
  /*for(int i=0;i<re2.size();i++)
    std::cout<<re2[i]<<std::endl;
    std::cout<<std::endl;*/
  REQUIRE(re3.size() == 3);
  /*for(int i=0;i<re3.size();i++)
    std::cout<<re3[i]<<std::endl;
  std::cout<<std::endl;
  */
}

TEST_CASE("2d triangle intersect2", "[2dti2]") {
  Real r1[2] = {1.0, 1.0}, r2[2] = {0.0, 0.0}, r3[2] = {2.0, -2.0},
       r4[2] = {0.5, 0.0}, r5[2] = {0.1, 0.0}, r6[2] = {0.5, -0.4};
  Point<Real, 2> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6);
  Point<Real, 2> tripoints1[] = {tp1, tp2, tp3}, tripoints2[] = {tp4, tp5, tp6};
  Triangle<Real, 2> tri1(tripoints1), tri2(tripoints2);
  std::vector<Segment<Real, 2>> re1;
  tri1.intersect(tri2, re1);
  for (int i = 0; i < re1.size(); i++)
    std::cout << re1[i] << std::endl;
  std::cout << std::endl;
  REQUIRE(re1.size() == 3);
}

TEST_CASE("3d Triangle intersect triangle  ", "[tit1]") {
  Real r1[3] = {1.0, 1.0, 0.0}, r2[3] = {0.0, 0.0, 0.0},
       r3[3] = {2.0, -2.0, 0.0}, r4[3] = {0.5, 1.0, 0.0},
       r5[3] = {0.5, -3.0, 0.0}, r6[3] = {4.0, 0.0, 0.0},
       r7[3] = {98.5, 99.5, 0.0}, r8[3] = {100.0, 1.0, 0.0},
       r9[3] = {220.5, 10.0, 0.0}, r10[3] = {0.4, 1.0, -1.0},
       r11[3] = {0.4, -2.0, -1.0}, r12[3] = {0.4, 0.0, 2.0},
       r13[3] = {0.5, 0.0, 0.0}, r14[3] = {0.1, 0.0, 0.0},
       r15[3] = {0.5, -0.4, 0.0};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6), tp7(r7),
      tp8(r8), tp9(r9), tp10(r10), tp11(r11), tp12(r12), tp13(r13), tp14(r14),
      tp15(r15);
  Point<Real, 3> tripoints1[] = {tp1, tp2, tp3}, tripoints2[] = {tp4, tp5, tp6},
                 tripoints3[] = {tp7, tp8, tp9},
                 tripoints4[] = {tp10, tp11, tp12},
                 tripoints5[] = {tp13, tp14, tp15};
  Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3),
      tri4(tripoints4), tri5(tripoints5);
  Triangle<Real, 2> proTri1 = tri1.project(2), proTri2 = tri2.project(2);
  std::vector<Segment<Real, 2>> re5;
  proTri1.intersect(proTri2, re5);
  REQUIRE(re5.size() == 6);
  for (int i = 0; i < re5.size(); i++)
    std::cout << re5[i] << std::endl;
  std::cout << std::endl;
  std::vector<Segment<Real, 3>> re1, re2, re3, re4;
  REQUIRE(tri1.intersect(tri2, re1) == Triangle<Real, 3>::intsType::Overlap);
  REQUIRE(re1.size() == 6);
  for (int i = 0; i < re1.size(); i++)
    std::cout << re1[i] << std::endl;
  std::cout << std::endl;
  REQUIRE(tri1.intersect(tri3, re2) == Triangle<Real, 3>::intsType::Never);
  REQUIRE(re2.size() == 0);
  REQUIRE(tri1.intersect(tri4, re3) == Triangle<Real, 3>::intsType::IntsSeg);
  std::cout << re3[0] << std::endl;
  std::cout << std::endl;
  REQUIRE(re3.size() == 1);
  REQUIRE(tri1.intersect(tri5, re4) == Triangle<Real, 3>::intsType::Overlap);
  std::cout << std::endl;
  for (int i = 0; i < re4.size(); i++)
    std::cout << re4[i] << std::endl;
  REQUIRE(re4.size() == 3);
}
