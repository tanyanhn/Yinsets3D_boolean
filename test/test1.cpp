#include "../catch.hpp"
#include "../src/YinSet.h"
#include <vector>
#include "importdata1.h"

using namespace YSB;
using namespace std;

TEST_CASE("monkey&&monkey", "[mm]")
{
    string smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
           smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
    // string steddy("..//dataset//teddy&cow//teddy.obj"),
    //     scow("dataset//teddy&cow//cow.obj");
    YinSet<Real> ymonkey1 = importdata1(smonkey1), ymonkey2 = importdata1(smonkey2);

    auto res = ymonkey1.meet(ymonkey2);
    REQUIRE(1 == 1);
}
