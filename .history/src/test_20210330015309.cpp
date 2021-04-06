#include "Point.h"
#include "PointCompare.h"
#include "Triangle.h"
#include <iostream>

using namespace YSB;
using namespace std;

int main(int argc, char const *argv[])
{
     Real r[] = {1.1, -3.1, 0.3};
     Point<Real, 3> p(r), o;
     auto v = p - o;
     v = v * 2;
     auto p1 = p + v;
     cout << "p : " << p << endl
          << "p1 : " << p1 << endl;
     auto p2 = p1.project(1), p3 = p.project(0);
     cout << "p2 : " << p2 << endl
          << "p3 : " << p3 << endl;

     Real rk = Real();
     float fk = float();
     int ik = int();
     cout << "rk : " << rk << endl;
     cout << "fk : " << fk << endl;
     cout << "ik : " << ik << endl;
     return 0;
}
