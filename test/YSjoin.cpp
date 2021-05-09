#include "../catch.hpp"
#include "../src/Totalheader.h"
#include <boost/timer/timer.hpp>

using namespace YSB;
using namespace std;

TEST_CASE("rabbit meet join 1", "[join]")
{
    // string s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
    //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
    // string smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
    //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
    // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
    //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
    string name1 = "Rabbit_Lowpoly_1",
           name2 = "Rabbit_Lowpoly_4",
           name3 = "ballabehind",
           name4 = "innerball",
           name5 = "innertorus",
           inpre = "../dataset/",
           oupre = "../res/",
           //     inpre = "dataset/",
        // oupre = "res/",
        post = ".obj",
           mid = "ballconnecttorus_torusbig_meet",
           mid2 = "complex",
           mid3 = "rabbit",
           mid4 = "heart",
           mid5 = "bust",
           mid6 = "ball",
           mid7 = "torus",
           m = "meet/",
           j = "join/",
           c = "complement/";

    int prec = 7;
    string s1 = inpre + mid3 + "/" + name1 + post,
           s2 = inpre + mid3 + "/" + name2 + post,
           s3 = inpre + mid2 + "/" + name3 + post,
           s4 = inpre + mid2 + "/" + name4 + post,
           s5 = inpre + mid2 + "/" + name5 + post;
    YinSet<Real> y1(s1, 1), y2(s2, 1);

    auto
        res1 = y1.join(y2),
        res2 = y1.meet(y2),
        res3 = y1.complement();

    res1.objOutput(name1 + "_" + name2,
                   oupre + name1 + "_" + name2 + "_" + j);

    REQUIRE(res1.isBounded() == 1);
    REQUIRE(res1.getBettiNumber(0) == 1);
    REQUIRE(res1.getBettiNumber(1) == 0);

    // cout << res1.getHasseString() << "\n";

    res2.objOutput(name1 + "_" + name2,
                   oupre + name1 + "_" + name2 + "_" + m);

    REQUIRE(res2.isBounded() == 1);
    REQUIRE(res2.getBettiNumber(0) == 1);
    REQUIRE(res2.getBettiNumber(1) == 2);

    // cout << y1.getHasseString() << "\n";
    // cout << y2.getHasseString() << "\n";
    res3.objOutput(name1,
                   oupre + name1 + "_" + c);

    REQUIRE(res3.isBounded() == 0);
    REQUIRE(res3.getBettiNumber(0) == 3);
    REQUIRE(res3.getBettiNumber(1) == 1);

    // cout << res3.getHasseString() << "/n";
}

// TEST_CASE("rabbit meet happy", "[join]")
// {
//     // string s1("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere1.obj"),
//     //        s2("D://github//Yinsets3D_boolean//dataset//icosphere//icosphere2.obj");
//     // string smonkey1("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey1.obj"),
//     //        smonkey2("D://github//Yinsets3D_boolean//dataset//teddy&monkey//monkey2.obj");
//     // string s1("D://github//Yinsets3D_boolean//dataset//teddy&cow//teddy.obj"),
//     //     s2("D://github//Yinsets3D_boolean//dataset//teddy&cow//cow.obj");
//     string name1 = "happy",
//            name2 = "rabbit5",
//            name3 = "ballabehind",
//            name4 = "innerball",
//            name5 = "innertorus",
//            inpre = "../dataset/",
//            oupre = "../res/",
//            post = ".obj",
//            post2 = ".ply",
//            mid = "happy",
//            mid2 = "complex",
//            mid3 = "rabbit",
//            mid4 = "heart",
//            mid5 = "bust",
//            mid6 = "ball",
//            mid7 = "torus",
//            m = "meet/",
//            j = "join/",
//            c = "complement/";

//     int prec = 7;
//     string s1 = inpre + mid + "/" + name1 + post2,
//            s2 = inpre + mid3 + "/" + name2 + post2;
//     boost::timer::auto_cpu_timer t, totaltime;

//     YinSet<Real> y1(s1, 2), y2(s2, 1);
//     std::cout << std::endl
//               << "y1, y2 construct cpu time : ";
//     t.~auto_cpu_timer();

//     boost::timer::auto_cpu_timer tMeet;
//     auto res1 = y1.meet(y2);
//     std::cout << std::endl
//               << "Meet cpu time : ";
//     tMeet.~auto_cpu_timer();

//     boost::timer::auto_cpu_timer tJoin;
//     auto res2 = y1.join(y2);
//     std::cout << std::endl
//               << "Join cpu time : ";
//     tJoin.~auto_cpu_timer();

//     cout << "y1 : \n"
//          << y1.getHasseString() << "\n";
//     cout << "y2 : \n"
//          << y2.getHasseString() << "\n";
//     cout << "res1 : \n"
//          << res1.getHasseString() << "\n";
//     cout << "res2 : \n"
//          << res2.getHasseString() << "\n";

//     res1.objOutput(name1 + "_" + name2,
//                    oupre + name1 + "_" + name2 + "_" + m);

//     res2.objOutput(name1 + "_" + name2,
//                    oupre + name1 + "_" + name2 + "_" + j);

//     REQUIRE(res1.isBounded() == 1);
//     // REQUIRE(res1.getBettiNumber(0) == 1);
//     // REQUIRE(res1.getBettiNumber(1) == 0);

//     REQUIRE(res2.isBounded() == 1);
//     // REQUIRE(res2.getBettiNumber(0) == 1);
//     // REQUIRE(res2.getBettiNumber(1) == 2);
//     std::cout << std::endl
//               << "happy rabbit total time : ";
//     totaltime.~auto_cpu_timer();
// }