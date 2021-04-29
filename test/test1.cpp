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
    string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
        s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
    YinSet<Real> y1 = importdata1(s1), y2 = importdata1(s2);

    auto res = y1.meet(y2);
    exportdata("../res/cowteddymeet", res);
    auto res2 = y1.join(y2);
    exportdata("../res/cowteddyjoin", res2);
    REQUIRE(1 == 1);
}
