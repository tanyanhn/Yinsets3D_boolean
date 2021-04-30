#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <random>

namespace YSB
{
    using Real = double;
    const Real TOL = 10e-10;
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
