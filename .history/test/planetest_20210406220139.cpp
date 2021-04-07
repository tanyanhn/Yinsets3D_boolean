#include <catch2/catch.hpp>
#include "../src/Plane.h"

using namespace YSB;

TEST_CASE("Plane line intersection1", "[pl1]")
{
  Real r1[3] = {1.0, 1.0, 1.0}, r2[3] = {0.0, 0.0, 1.0}, r3[3] = {2.0, 2.4, 1.3}, r4[3] = {2.0, 2.4, 1.0};
  Point<Real, 3> pp1(r1), fp1(r3);
  Vec<Real, 3> d1(r2);
  Plane<Real> pla1(pp1, d1);
  Line<Real, 3> l1(fp1, d1);
  Point<Real, 3> rp = pla1.intersect(l1), ap(r4);
  REQUIRE(norm(rp - ap) < TOL);
}

TEST_CASE("Plane line intersection2", "[pl2]")
{
  Real r1[3] = {0.0, 0.0, 0.0}, r2[3] = {1.0, 1.0, 1.0}, r3[3] = {2.0, 2.5, 1.5}, r4[3] = {-6.0 / 7.0 + 2.0, -12.0 / 7.0 + 2.5, -24.0 / 7.0 + 1.5}, r5[3] = {0.5, 1.0, 2.0};
  Point<Real, 3> pp1(r1), fp1(r3);
  Vec<Real, 3> d1(r2), d2(r5);
  Plane<Real> pla1(pp1, d1);
  Line<Real, 3> l1(fp1, d2);
  Point<Real, 3> rp = pla1.intersect(l1), ap(r4);
  REQUIRE(norm(rp - ap) < TOL);
}

TEST_CASE("Plane plane intersection1", "[pp1]")
{
  Real r1[3] = {0.0, 0.0, 0.0}, r2[3] = {0.0, 0.0, 1.0}, r3[3] = {1.0, 2.0, 1.5}, r4[3] = {1.5, 0.0, 0.0}, r5[3] = {1.0, 0.0, 0.0}, r6[3] = {0.0, 1.0, 0.0};
  Point<Real, 3> pp1(r1), pp2(r3), fp3(r5);
  Vec<Real, 3> d1(r2), d2(r4), d3(r6);
  Plane<Real> pla1(pp1, d1), pla2(pp2, d2);
  Line<Real, 3> l1(fp3, d3), l2 = pla1.intersect(pla2);
  l2.moveFixpoint(l1.fixpoint[l1.majorDim()], l1.majorDim());
  REQUIRE(norm(l1.fixpoint - l2.fixpoint) < TOL);
  REQUIRE(norm(normalize(l1.direction) - normalize(l2.direction)) < TOL);
}

TEST_CASE("Plane plane intersection2", "[pp2]")
{
  Real r1[3] = {0.0, 0.0, 0.0}, r2[3] = {0.5, 0.5, 1.0}, r3[3] = {2.0, 2.0, 1.5}, r4[3] = {2.0, -1.0, 0.0}, r5[3] = {0.0, -2.0, 1.0}, r6[3] = {1.0, 2.0, -1.5};
  Point<Real, 3> pp1(r1), pp2(r3), fp3(r5);
  Vec<Real, 3> d1(r2), d2(r4), d3(r6);
  Plane<Real> pla1(pp1, d1), pla2(pp2, d2);
  Line<Real, 3> l1(fp3, d3), l2 = pla1.intersect(pla2);
  l2.moveFixpoint(l1.fixpoint[l1.majorDim()], l1.majorDim());
  std::cout << l2.fixpoint << " " << l1.fixpoint << std::endl;
  REQUIRE(norm(l1.fixpoint - l2.fixpoint) < TOL);
  REQUIRE(norm(normalize(l1.direction) - normalize(l2.direction)) < TOL);
}
