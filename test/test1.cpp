#include "../catch.hpp"
#include "../src/YinSet.h"
#include <vector>
#include "importdata.h"

using namespace YSB;
using namespace std;

TEST_CASE("teddy&&cow", "[tc]")
{
    // string steddy("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
    //        scow("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
    string steddy("../dataset/teddy&cow/teddy.obj"),
        scow("../dataset/teddy&cow/cow.obj");
    YinSet<Real> ycow = importdata(scow), yteddy = importdata(steddy);

    auto res = yteddy.meet(ycow);
    REQUIRE(1 == 1);
}
