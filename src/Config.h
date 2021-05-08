#ifndef CONFIGN_H
#define CONFIGN_H

#include <iostream>
#include <random>

#ifdef _unix
#define _UsBoost
#endif // _linux

#ifdef _intersectTest
#define _bottleneck_
#endif // _intersectTest

namespace YSB
{
    using Real = double;
#ifdef _precise10
    const Real TOL = 1e-10;
#else
    const Real TOL = 1e-5;
#endif // _precise10

    const int spaceDim = 3;
    const Real GreatValue = 10e10;
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> realdistrib(0, 1);

} // namespace YSB

#endif // CONFIG_H
