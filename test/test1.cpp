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
    string name1 = "teddy",
           name2 = "cow",
           inpre = "../dataset/",
           oupre = "../res/",
           post = ".obj",
           mid = "teddy&cow",
           mid2 = "rabbit",
           m = "meet/",
           j = "join/",
           c = "complement/";
    string s1 = inpre + mid + "/" + name1 + post,
           s2 = inpre + mid + "/" + name2 + post;
    YinSet<Real> y1 = importdata1(s1), y2 = importdata1(s2);

    auto res = y1.meet(y2);
    exportdata(name1 + "_" + name2, res,
               oupre + name1 + "_" + name2 + "_" + m);
    // auto res2 = y1.join(y2);
    // exportdata("../res/cowteddyjoin", res2);
    REQUIRE(1 == 1);
}
