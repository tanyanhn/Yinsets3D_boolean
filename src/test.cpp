#include <iostream>
#include <vector>
#include "Point.h"
#include "PointCompare.h"
#include "Triangle.h"

using namespace YSB;
using namespace std;

class A {
  double k = -0.4;
  int i;

 public:
  A(int ii = 3) : i(ii) {}
  void print() const { cout << "k = " << k << endl << "i = " << i << endl; }
};

int main() {
  A a, b(10);
  a.print();
  b.print();
}
