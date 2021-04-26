#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>

namespace YSB
{
    using Real = double;
    const Real TOL = 10e-8;
    const int spaceDim = 3;
    const Real GreatValue = 10e10;
    #ifndef M_PI
    #define M_PI 3.14159265358979323846
    #endif
} // namespace YSB

#endif // CONFIG_H
