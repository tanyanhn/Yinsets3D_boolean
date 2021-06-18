#include <vector>
#include "../catch.hpp"
#include "../src/Totalheader.h"
#include "../src/TriangleCompare.h"
#include <iostream>

using namespace YSB;
using namespace std;

// TEST_CASE("YinSet meet 1", "[meet]") {
//   // string
//   // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
//   //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
//   // string
//   // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
//   //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
//   // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
//   //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
//   string name1 = "torus", name2 = "ballahead", name3 = "ballabehind",
//          name4 = "innerball", name5 = "innertorus", inpre = "../dataset/",
//          oupre = "../res/",
//          //     inpre = "dataset/",
//       // oupre = "res/",
//       post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
//          mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
//          mid7 = "torus", m = "meet/", j = "join/", c = "complement/";
//   string s1 = inpre + mid2 + "/" + name1 + post,
//          s2 = inpre + mid2 + "/" + name2 + post,
//          s3 = inpre + mid2 + "/" + name3 + post,
//          s4 = inpre + mid2 + "/" + name4 + post,
//          s5 = inpre + mid2 + "/" + name5 + post;
//   YinSet<Real> y1(s1), y2(s2), y3(s3), y4(s4), y5(s5);

//   auto res = y1.meet(y4.complement()).meet(y5.complement());
//   // .meet(y5.complement());

//   res.objOutput("torus2hole", inpre + mid2 + "/", 5);

//   REQUIRE(res.isBounded() == 1);
//   REQUIRE(res.getBettiNumber(0) == 1);
//   REQUIRE(res.getBettiNumber(1) == 2);
// }

// TEST_CASE("YinSet meet 2", "[meet]") {
//   // string
//   // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
//   //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
//   // string
//   // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
//   //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
//   // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
//   //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
//   string name1 = "torus2hole", name2 = "ballabehind", name3 = "ballahead",
//          name4 = "innerball", name5 = "innertorus", inpre = "../dataset/",
//          oupre = "../res/",
//          //     inpre = "dataset/",
//       // oupre = "res/",
//       post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
//          mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
//          mid7 = "torus", m = "meet/", j = "join/", c = "complement/";
//   string s1 = inpre + mid2 + "/" + name1 + post,
//          s2 = inpre + mid2 + "/" + name2 + post,
//          s3 = inpre + mid2 + "/" + name3 + post,
//          s4 = inpre + mid2 + "/" + name4 + post,
//          s5 = inpre + mid2 + "/" + name5 + post;
//   YinSet<Real> y1(s1), y2(s2), y3(s3);

//   auto res1 = y1.meet(y2);

//   res1.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + m);

//   REQUIRE(res1.isBounded() == 1);
//   REQUIRE(res1.getBettiNumber(0) == 1);
//   REQUIRE(res1.getBettiNumber(1) == 1);
// }

// TEST_CASE("YinSet meet 3", "[meet]") {
//   // string
//   // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
//   //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
//   // string
//   // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
//   //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
//   // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
//   //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
//   string name1 = "torus2hole", name2 = "ballabehind", name3 = "ballahead",
//          name4 = "innerball", name5 = "innertorus", inpre = "../dataset/",
//          oupre = "../res/",
//          //     inpre = "dataset/",
//       // oupre = "res/",
//       post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
//          mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
//          mid7 = "torus", m = "meet/", j = "join/", c = "complement/";
//   string s1 = inpre + mid2 + "/" + name1 + post,
//          s2 = inpre + mid2 + "/" + name2 + post,
//          s3 = inpre + mid2 + "/" + name3 + post,
//          s4 = inpre + mid2 + "/" + name4 + post,
//          s5 = inpre + mid2 + "/" + name5 + post;
//   YinSet<Real> y1(s1), y2(s2), y3(s3);

//   auto res1 = y1.meet(y3);

//   res1.objOutput(name1 + "_" + name3, oupre + name1 + "_" + name3 + "_" + m);

//   REQUIRE(res1.isBounded() == 1);
//   REQUIRE(res1.getBettiNumber(0) == 2);
//   REQUIRE(res1.getBettiNumber(1) == 0);
// }


// TEST_CASE("YinSet meet 4", "[meet]") {
//   // string
//   // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
//   //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
//   // string
//   // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
//   //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
//   // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
//   //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
//   string name1 = "torus2hole", name2 = "ballabehind", name3 = "ballahead",
//          name4 = "innerball", name5 = "innertorus", name6 = "innertorusball", 
//          name7 = "torus",inpre = "../dataset/", oupre = "../res/",
//          //     inpre = "dataset/",
//       // oupre = "res/",
//       post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
//          mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
//          mid7 = "torus", mid8 = "complex1", m = "meet/", j = "join/", c = "complement/";
//   string s1 = inpre + mid8 + "/" + name6 + post,
//          s2 = inpre + mid8 + "/" + name7 + post;
//   YinSet<Real> y1(s1), y2(s2);

//   auto res1 = y2.meet(y1.complement());

//   res1.objOutput(name6 + "_" + name7, oupre + name6 + "_" + name7 + "_" + m);

//   REQUIRE(res1.isBounded() == 1);
//   REQUIRE(res1.getBettiNumber(0) == 1);
//   REQUIRE(res1.getBettiNumber(1) == 2);
// }

// TEST_CASE("YinSet meet 5", "[meet]") {
//   // string
//   // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
//   //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
//   // string
//   // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
//   //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
//   // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
//   //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
//   string name1 = "torus2hole", name2 = "ballabehind", name3 = "ballahead",
//          name4 = "innerball", name5 = "innertorus", name6 = "twoballs", 
//          name7 = "torus1",inpre = "../dataset/", oupre = "../res/",
//          //     inpre = "dataset/",
//       // oupre = "res/",
//       post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
//          mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
//          mid7 = "torus", mid8 = "complex1", m = "meet/", j = "join/", c = "complement/";
//   string s1 = inpre + mid8 + "/" + name6 + post,
//          s2 = inpre + mid8 + "/" + name7 + post;
//   YinSet<Real> y1(s1), y2(s2);
// cout<<y1.gcss().size()<<endl<<y2.gcss().size()<<endl;
//   auto res1 = y2.meet(y1.complement());

//   res1.objOutput(name6 + "_" + name7, oupre + name6 + "_" + name7 + "_" + m);

//   REQUIRE(res1.isBounded() == 1);
//   REQUIRE(res1.getBettiNumber(0) == 2);
//   REQUIRE(res1.getBettiNumber(1) == 0);
// }



// // 



// TEST_CASE("YinSet meet 6", "[meet]") {
//   // string
//   // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
//   //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
//   // string
//   // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
//   //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
//   // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
//   //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
//   string name1 = "rabbit1", name2 = "rabbit2", name3 = "ballahead",
//          name4 = "innerball", name5 = "innertorus", name6 = "innertorusball", 
//          name7 = "torus",inpre = "../dataset/", oupre = "../res/",
//          //     inpre = "dataset/",
//       // oupre = "res/",
//       post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
//          mid3 = "rabbit1", mid4 = "heart", mid5 = "bust", mid6 = "ball",
//          mid7 = "torus", mid8 = "complex1", m = "meet/", j = "join/", c = "complement/";
//   string s1 = inpre + mid3 + "/" + name2 + post,
//          s2 = inpre + mid3 + "/" + name1 + post;
//   YinSet<Real> y1(s1), y2(s2);
//   cout<<y1.gcss().size()<<endl<<y2.gcss().size()<<endl;
//   auto res1 = y2.meet(y1);

//   res1.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + m);

//   REQUIRE(res1.isBounded() == 1);
//   REQUIRE(res1.getBettiNumber(0) == 1);
//   REQUIRE(res1.getBettiNumber(1) == 0);
// }



// TEST_CASE("YinSet meet 7", "[meet]") {
//   // string
//   // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
//   //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
//   // string
//   // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
//   //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
//   // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
//   //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
//   string name1 = "torus2hole", name2 = "underball", name3 = "overball",
//          name4 = "innerball", name5 = "innertorus", name6 = "twoballs", 
//          name7 = "torus1",inpre = "../dataset/", oupre = "../res/",
//          //     inpre = "dataset/",
//       // oupre = "res/",
//       post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
//          mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
//          mid7 = "complex", mid8 = "complex1", m = "meet/", j = "join/", c = "complement/";
//   string s1 = inpre + mid7 + "/" + name1 + post,
//          s2 = inpre + mid8 + "/" + name2 + post;
//   YinSet<Real> y1(s1), y2(s2);
// cout<<y1.gcss().size()<<endl<<y2.gcss().size()<<endl;
//   auto res1 = y2.meet(y1);

//   res1.objOutput(name2 + "_" + name1, oupre + name2 + "_" + name1 + "_" + m);

//   REQUIRE(res1.isBounded() == 1);
//   REQUIRE(res1.getBettiNumber(0) == 1);
//   REQUIRE(res1.getBettiNumber(1) == 1);
// }


// TEST_CASE("YinSet meet 8", "[meet]") {
//   // string
//   // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
//   //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
//   // string
//   // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
//   //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
//   // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
//   //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
//   string name1 = "torus2hole", name2 = "underball", name3 = "overball",
//          name4 = "innerball", name5 = "innertorus", name6 = "twoballs", 
//          name7 = "torus1",inpre = "../dataset/", oupre = "../res/",
//          //     inpre = "dataset/",
//       // oupre = "res/",
//       post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
//          mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
//          mid7 = "complex", mid8 = "complex1", m = "meet/", j = "join/", c = "complement/";
//   string s1 = inpre + mid7 + "/" + name1 + post,
//          s2 = inpre + mid8 + "/" + name3 + post;
//   YinSet<Real> y1(s1), y2(s2);
// cout<<y1.gcss().size()<<endl<<y2.gcss().size()<<endl;
//   auto res1 = y2.meet(y1);

//   res1.objOutput(name3 + "_" + name1, oupre + name3 + "_" + name1 + "_" + m);

//   REQUIRE(res1.isBounded() == 1);
//   REQUIRE(res1.getBettiNumber(0) == 2);
//   REQUIRE(res1.getBettiNumber(1) == 0);
// }

TEST_CASE("YinSet meet 9", "[meet]") {
  // string
  // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
  //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
  // string
  // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
  //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
  // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
  //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
  string name1 = "balltorus1", name2 = "torus3", name3 = "overball",
         name4 = "innerball", name5 = "innertorus", name6 = "twoballs2", 
         name7 = "torus3",inpre = "../dataset/", oupre = "../res/",
         //     inpre = "dataset/",
      // oupre = "res/",
      post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
         mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
         mid7 = "complex", mid8 = "complex1", m = "meet/", j = "join/", c = "complement/";
  string s1 = inpre + mid8 + "/" + name1 + post,
         s2 = inpre + mid8 + "/" + name2 + post;
  YinSet<Real> y1(s1), y2(s2);
  
 cout << y1.complement().getHasseString() << "\n";
  cout << y2.getHasseString() << "\n";

  auto res1 = y2.meet(y1.complement());
cout << res1.getHasseString() << "\n";
  res1.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + m);

  REQUIRE(res1.isBounded() == 1);
  REQUIRE(res1.getBettiNumber(0) == 1); 
  REQUIRE(res1.getBettiNumber(1) == 2);


}  

TEST_CASE("YinSet meet 10", "[meet]") {
  // string
  // s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
  //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
  // string
  // smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
  //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
  // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
  //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
  string name1 = "torus2holes", name2 = "underball1", name3 = "overball",
         name4 = "innerball", name5 = "innertorus", name6 = "twoballs2", 
         name7 = "torus2",inpre = "../dataset/", oupre = "../res/",
         //     inpre = "dataset/",
      // oupre = "res/",
      post = ".obj", mid = "ballconnecttorus_torusbig_meet", mid2 = "complex",
         mid3 = "rabbit", mid4 = "heart", mid5 = "bust", mid6 = "ball",
         mid7 = "complex", mid8 = "complex1", m = "meet/", j = "join/", c = "complement/";
  string s1 = inpre + mid8 + "/" + name1 + post,
         s2 = inpre + mid8 + "/" + name2 + post;
  YinSet<Real> y1(s1), y2(s2);
  
 cout << y1.getHasseString() << "\n";
  cout << y2.getHasseString() << "\n";

  auto res1 = y2.meet(y1);
cout << res1.getHasseString() << "\n";
  res1.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + m);

  REQUIRE(res1.isBounded() == 1);
  REQUIRE(res1.getBettiNumber(0) == 2);
  REQUIRE(res1.getBettiNumber(1) == 0);


  auto res2 = y2.join(y1);
  cout << res2.getHasseString() << "\n";
  res2.objOutput(name1 + "_" + name2, oupre + name1 + "_" + name2 + "_" + j);

  REQUIRE(res2.isBounded() == 1);
  REQUIRE(res2.getBettiNumber(0) == 1);
  REQUIRE(res2.getBettiNumber(1) == 2);
}  
  

//  