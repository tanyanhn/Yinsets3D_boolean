#include "Point.h"
#include "PointCompare.h"
#include "Triangle.h"
#include <iostream>
#include <vector>

using namespace YSB;
using namespace std;

int main(int argc, char const *argv[])
{
     Real r1[] = {1.0, -3.0, 0.0};
     Real r2[] = {1.0, 3.0, 0.0}, r3[] = {1.0, 0.0, 0.0};
     Point<Real, 3> p1(r1), p2(r2), p3(0.0), p4(r3);
     Point<Real,3>vertex[] = {p1,p2,p3};
     Triangle<Real, 3> tri(vertex);
     Real co[3];
     cout<<tri.barycentric(p4, co)<<endl;
     cout<<co[0]<<" "<<co[1]<<" "<<co[2]<<endl;
     cout<<tri.barycentric(co)<<endl;
     Point<Real,3> p(0);
}
