#include "../catch.hpp"
#include "../src/YinSet.h"
#include <vector>
#include "importdata1.h"
#include "exportdata.h"

using namespace YSB;
using namespace std;

TEST_CASE("monkey&&monkey", "[mm]")
{
       // string s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
       //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
       // string smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
       //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
       // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
       //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
       string name1 = "icosphere",
              name2 = "Rabbit_Lowpoly_4",
              inpre = "../dataset/",
              oupre = "../res/",
              //     inpre = "dataset/",
           // oupre = "res/",
           post = ".obj",
              mid = "icosphere",
              mid2 = "teddy&cow",
              mid3 = "rabbit",
              mid4 = "heart",
              mid5 = "bust",
              m = "meet/",
              j = "join/",
              c = "complement/";
       string s1 = inpre + mid + "/" + name1 + post,
              s2 = inpre + mid3 + "/" + name2 + post;
       YinSet<Real> y1 = importdata1(s1), y2 = importdata1(s2);

       auto res = y1.join(y2);
       // auto res = y2.complement();
       exportdata(name1 + "_" + name2, res,
                  oupre + name1 + "_" + name2 + "_" + j);
       // exportdata(name2, res,
       //     oupre + name2 + "_" + c);
       // auto res2 = y1.join(y2);
       // exportdata("../res/cowteddyjoin", res2);
       auto k = res.isBounded();
       REQUIRE(res.isBounded() == 1);
       REQUIRE(1 == 1);
}
