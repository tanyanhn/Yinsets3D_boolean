#include <vector>
#include "../catch.hpp"
#include "../src/Totalheader.h"

using namespace YSB;
using namespace std;

TEST_CASE("ball and torus 1", "[rF::bt1]") {
  // string
  // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
  //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
  // string
  // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
  //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
  // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
  //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
  string name1 = "ballmid", name2 = "torusmid", inpre = "../dataset/",
         oupre = "../res/",
         //     inpre = "dataset/",
      // oupre = "res/",
      post = ".obj", mid = "icosphere", mid2 = "teddy&cow", mid3 = "rabbit",
         mid4 = "heart", mid5 = "bust", mid6 = "ball", mid7 = "torus",
         m = "meet/", j = "join/", c = "complement/";
  string s1 = inpre + mid6 + "/" + name1 + post,
         s2 = inpre + mid7 + "/" + name2 + post;
  YinSet<Real> y1(s1), y2(s2);
  auto y3 = y2.complement();
  auto res = y1.meet(y3, 1);

  res.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + m);

  REQUIRE(res.isBounded() == 1);
  REQUIRE(res.getBettiNumber(0) == 1);
  REQUIRE(res.getBettiNumber(1) == 1);
  REQUIRE(1 == 1);
}

TEST_CASE("torus contain 1 ball", "[rF::bt2]") {
  // string
  // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
  //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
  // string
  // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
  //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
  // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
  //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
  string name1 = "ballleft", name2 = "torusbig", inpre = "../dataset/",
         oupre = "../res/",
         //     inpre = "dataset/",
      // oupre = "res/",
      post = ".obj", mid = "icosphere", mid2 = "teddy&cow", mid3 = "rabbit",
         mid4 = "heart", mid5 = "bust", mid6 = "ball", mid7 = "torus",
         m = "meet/", j = "join/", c = "complement/";
  string s1 = inpre + mid6 + "/" + name1 + post,
         s2 = inpre + mid7 + "/" + name2 + post;
  YinSet<Real> y1(s1), y2(s2);
  auto y3 = y1.complement();
  auto res = y2.meet(y3, 1);
  // auto res = y2.complement();
  res.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + m);
  // exportdata(name2, res,
  //     oupre + name2 + "_" + c);
  // auto res2 = y1.join(y2);
  // exportdata("../res/cowteddyjoin", res2);
  REQUIRE(res.isBounded() == 1);
  REQUIRE(res.getBettiNumber(0) == 1);
  REQUIRE(res.getBettiNumber(1) == 1);
  REQUIRE(1 == 1);
}

TEST_CASE("torus contain 2 balls", "[rF::bt3]") {
  // string
  // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
  //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
  // string
  // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
  //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
  // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
  //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
  string name1 = "ballleft", name2 = "torusbig", name3 = "ballright",
         inpre = "../dataset/", oupre = "../res/",
         //     inpre = "dataset/",
      // oupre = "res/",
      post = ".obj", mid = "icosphere", mid2 = "teddy&cow", mid3 = "rabbit",
         mid4 = "heart", mid5 = "bust", mid6 = "ball", mid7 = "torus",
         m = "meet/", j = "join/", c = "complement/";
  string s1 = inpre + mid6 + "/" + name1 + post,
         s2 = inpre + mid7 + "/" + name2 + post,
         s3 = inpre + mid6 + "/" + name3 + post;
  YinSet<Real> y1(s1), y2(s2), y3(s3);
  auto y4 = y1.join(y3, 1).complement(1);
  auto res = y2.meet(y4, 1);
  // auto res = y2.complement();
  res.objOutput(name1 + "_" + name2,
                oupre + name1 + "_and_" + name3 + "_" + name2 + "_" + m);
  // exportdata(name2, res,
  //     oupre + name2 + "_" + c);
  // auto res2 = y1.join(y2);
  // exportdata("../res/cowteddyjoin", res2);
  REQUIRE(res.isBounded() == 1);
  REQUIRE(res.getBettiNumber(0) == 2);
  REQUIRE(res.getBettiNumber(1) == 0);
  REQUIRE(1 == 1);
}

TEST_CASE("torus contain a ball connect torus", "[rF::bt4]") {
  // string
  // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
  //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
  // string
  // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
  //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
  // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
  //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
  string name1 = "ballconnecttorus", name2 = "torusbig", name3 = "ballright",
         inpre = "../dataset/", oupre = "../res/",
         // inpre = "dataset/",
      // oupre = "res/",
      post = ".obj", mid = "icosphere", mid2 = "teddy&cow", mid3 = "rabbit",
         mid4 = "heart", mid5 = "bust", mid6 = "ball", mid7 = "torus",
         m = "meet/", j = "join/", c = "complement/";
  string s1 = inpre + mid6 + "/" + name1 + post,
         s2 = inpre + mid7 + "/" + name2 + post,
         s3 = inpre + mid6 + "/" + name3 + post;
  YinSet<Real> y1(s1), y2(s2);
  auto res = y2.meet(y1.complement(1), 1);

  res.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + m);

  REQUIRE(res.isBounded() == 1);
  REQUIRE(res.getBettiNumber(0) == 1);
  REQUIRE(res.getBettiNumber(1) == 1);
  REQUIRE(1 == 1);
}

TEST_CASE("torus contain a torus", "[rF::bt5]") {
  // string
  // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
  //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
  // string
  // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
  //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
  // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
  //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
  string name1 = "toruscontainor", name2 = "toruscontained",
         name3 = "ballright", inpre = "../dataset/", oupre = "../res/",
         //     inpre = "dataset/",
      // oupre = "res/",
      post = ".obj", mid = "icosphere", mid2 = "teddy&cow", mid3 = "rabbit",
         mid4 = "heart", mid5 = "bust", mid6 = "ball", mid7 = "torus",
         m = "meet/", j = "join/", c = "complement/";
  string s1 = inpre + mid7 + "/" + name1 + post,
         s2 = inpre + mid7 + "/" + name2 + post,
         s3 = inpre + mid6 + "/" + name3 + post;
  YinSet<Real> y1(s1), y2(s2);
  auto res = y1.meet(y2.complement(1), 1);
  // auto res = y2.complement();
  res.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + m);
  // exportdata(name2, res,
  //     oupre + name2 + "_" + c);
  // auto res2 = y1.join(y2);
  // exportdata("../res/cowteddyjoin", res2);
  REQUIRE(res.isBounded() == 1);
  REQUIRE(res.getBettiNumber(0) == 2);
  REQUIRE(res.getBettiNumber(1) == 0);
  REQUIRE(1 == 1);
}

TEST_CASE("torus contain a torus 2", "[rF::bt5]") {
  // string
  // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
  //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
  // string
  // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
  //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
  // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
  //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
  string name1 = "ballconnecttorus_torusbig",
         name2 = "ballconnecttorus_torusbig1", name3 = "ballright",
         inpre = "../dataset/", oupre = "../res/",
         //     inpre = "dataset/",
      // oupre = "res/",
      post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "teddy&cow",
         mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
         mid7 = "torus", m = "meet/", j = "join/", c = "complement/";
  string s1 = oupre + mid + "/" + name1 + post,
         s2 = oupre + mid + "/" + name2 + post,
         s3 = inpre + mid6 + "/" + name3 + post;
  YinSet<Real> y1(s1);
  auto res = y1.complement();
  // auto res = y2.complement();
  res.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + m + c);
  // exportdata(name2, res,
  //     oupre + name2 + "_" + c);
  // auto res2 = y1.join(y2);
  // exportdata("../res/cowteddyjoin", res2);
  REQUIRE(res.isBounded() == 0);
  REQUIRE(res.getBettiNumber(0) == 2);
  REQUIRE(res.getBettiNumber(1) == 1);
  REQUIRE(1 == 1);
}
