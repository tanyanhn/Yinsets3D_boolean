#include <boost/timer/config.hpp>
#include <boost/timer/timer.hpp>
#include <string>
#include "../catch.hpp"
#include "../src/OctreeTriangleIntersection.h"
#include "../src/Totalheader.h"
#include "../src/TriangleIntersection.h"

using namespace YSB;
using namespace std;

// TEST_CASE("edge coincident find", "[intersect]") {
//   string name1 = "torus2hole", name2 = "tbs_t", name3 = "ballahead",
//          name4 = "innerball", name5 = "innertorus", name6 = "box", name7 =
//          "t", inpre = "../dataset/", oupre = "../res/",
//          //     inpre = "dataset/",
//       // oupre = "res/",
//       post = ".obj", mid = "tbs_t_meet", mid2 = "complex", mid3 = "rabbit",
//          mid4 = "heart", mid5 = "bust", mid6 = "ball", mid7 = "tbs_t_meet",
//          mid8 = "Octree", m = "meet/", j = "join/", c = "complement/";
//   string s1 = oupre + mid8 + "/" + name6 + post,
//          s2 = oupre + mid7 + "/" + name2 + post;
//   Real tol = 2e-5;
//   YinSet<Real> y1(s1, 0, tol), y2(s2, 0, tol);
//   std::vector<Triangle<Real, 3>> inputA;
//   y1.collapse(inputA, 1, tol);
//   OctreeTriangleIntersection<Real> intersectOp;
//   intersectOp.edgeCal(inputA, tol, 1);
//   int num = inputA.size();
//   for (int i = 0; i < num; ++i) {
//     REQUIRE(intersectOp.resultA[i].first.size() == 3);
//     for (auto seg : intersectOp.resultA[i].first) {
//       REQUIRE(inputA[i].edgeVec(seg) != -1);
//       REQUIRE(seg.neighborhood().size() == 2);
//     }
//   }
//   intersectOp.resultA.clear();
//   inputA.clear();
//   y2.collapse(inputA, 1, tol);
//   intersectOp.edgeCal(inputA, tol, 1);
//   num = inputA.size();
//   for (int i = 0; i < num; ++i) {
//     REQUIRE(intersectOp.resultA[i].first.size() == 3);
//     for (auto seg : intersectOp.resultA[i].first) {
//       REQUIRE(inputA[i].edgeVec(seg) != -1);
//       REQUIRE(
//           (seg.neighborhood().size() == 2 || seg.neighborhood().size() ==
//           4));
//     }
//   }
// }

void dfs(OctreeNode<Cuboid<Real>>* r, YinSet<Real>& y) {
  if (!r->child.empty()) {
    for (auto subr : r->child) {
      dfs(subr, y);
    }
  } else {
    r->val.covertYin(y);
  }
}

// TEST_CASE("Octree Construct", "[intersect]") {
//   string name1 = "b3", name2 = "t3", name3 = "tb3", name4 = "innerball",
//          name5 = "innertorus", name6 = "box", name7 = "t",
//          inpre = "../dataset/", oupre = "../res/", post = ".obj",
//          mid = "tbs_t_meet", mid2 = "complex", mid3 = "rabbit", mid4 =
//          "heart", mid5 = "bust", mid6 = "ball", mid7 = "tbs_t_meet", mid8 =
//          "Octree", m = "meet/", j = "join/", c = "complement/";
//   string s1 = oupre + mid8 + "/" + name1 + post,
//          s2 = oupre + mid8 + "/" + name2 + post,
//          s3 = oupre + mid8 + "/" + name3 + post;
//   Real tol = 2e-5;
//   YinSet<Real> y1(s1, 0, tol), y2(s2, 0, tol), y3;

//   std::vector<Triangle<Real, 3>> inputA, inputB;
//   y1.collapse(inputA, 1, tol);
//   y2.collapse(inputB, 2, tol);
//   OctreeTriangleIntersection<Real> intersectOp;
//   long num = inputA.size() + inputB.size(), deep = std::log2(num) / 3;
//   intersectOp.initOctree(inputA, inputB, deep, tol);
//   intersectOp.pruneTree(intersectOp.root);
//   dfs(intersectOp.root, y3);
//   y3.objOutput(name3, oupre + mid8 + "/");
// }

TEST_CASE("Octree Intersection") {
  string name1 = "b3", name2 = "t3", name3 = "tb3", name4 = "innerball",
         name5 = "innertorus", name6 = "box", name7 = "t",
         inpre = "../dataset/", oupre = "../res/", post = ".obj",
         mid = "tbs_t_meet", mid2 = "complex", mid3 = "rabbit", mid4 = "heart",
         mid5 = "bust", mid6 = "ball", mid7 = "tbs_t_meet", mid8 = "Octree",
         m = "meet/", j = "join/", c = "complement/";
  for (int i = 1; i < 5; ++i) {
    string s1 = oupre + mid8 + "/b" + to_string(i) + post,
           s2 = oupre + mid8 + "/t" + to_string(i) + post,
           s3 = oupre + mid8 + "/tb" + to_string(i) + post;
    Real tol = 2e-5;
    YinSet<Real> y1(s1, 0, tol), y2(s2, 0, tol), y3;

    std::vector<Triangle<Real, 3>> inputA, inputB;
    y1.collapse(inputA, 1, tol);
    y2.collapse(inputB, 2, tol);
    long nA = inputA.size(), nB = inputB.size(), pA = -1, pB = -1;
    long num = inputA.size() + inputB.size(), deep = std::log2(num) / 3;
    std::cout << "\n num of tris:(" << inputA.size() << ", " << inputB.size()
              << "), " << num << ", Octree deep:" << deep << endl;
    OctreeTriangleIntersection<Real> intersectOp;
    {
      // #ifdef _UsBoost
      boost::timer::auto_cpu_timer t;
      std::cout << "OctreeIntersect cpu time : ";

      // #endif  // _UsBoost

      long num = inputA.size() + inputB.size(), deep = std::log2(num) / 3;
      intersectOp(inputA, inputB, tol);
    }
    TriangleIntersection<Real> intersectOp2;
    {
      // #ifdef _UsBoost
      boost::timer::auto_cpu_timer t;
      std::cout << "normalIntersect cpu time : ";
      // #endif  // _UsBoost

      intersectOp2(inputA, inputB, tol);
    }
    // for (int i = 0; i < nA; ++i) {
    //   set<Segment<Real, 3>, SegmentCompare> segs1, segs2;
    //   for (auto seg : intersectOp.resultA[i].first)
    //     segs1.insert(seg);
    //   for (auto seg : intersectOp2.resultA[i].first)
    //     segs2.insert(seg);
    //   vector<Segment<Real, 3>> se1(segs1.begin(), segs1.end()),
    //       se2(segs2.begin(), segs2.end());
    //   REQUIRE(
    //       (segs1.size() >= segs2.size() &&
    //        intersectOp.TriangleIntersection<Real>::resultA[i].second.size()
    //        ==
    //            intersectOp2.resultA[i].second.size()));
    // }
    // for (int i = 0; i < nA; ++i) {
    //   set<Segment<Real, 3>, SegmentCompare> segs1, segs2;
    //   for (auto seg : intersectOp.resultB[i].first)
    //     segs1.insert(seg);
    //   for (auto seg : intersectOp2.resultB[i].first)
    //     segs2.insert(seg);
    //   REQUIRE(
    //       (segs1.size() >= segs2.size() &&
    //        intersectOp.TriangleIntersection<Real>::resultB[i].second.size()
    //        ==
    //            intersectOp2.resultB[i].second.size()));
    // }
  }
}