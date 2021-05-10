#include <fstream>
#include <iostream>
#include <vector>
#include "../src/YinSet.h"

using namespace std;
using namespace YSB;

inline YinSet<Real> importdata(string s) {
  ifstream infile(s);
  if (infile.is_open()) {
    vector<Triangle<Real, 3>> vectri;
    vector<Point<Real, 3>> vecP;
    int size = 0;
    while (!infile.eof()) {
      char c;
      int index[3];
      infile >> c;
      if (infile.fail()) {
        break;
      }
      if (c == 'v') {
        Real p[3];
        infile >> p[0] >> p[1] >> p[2];
        vecP.emplace_back(p);
        size++;
      } else if (c == 'f') {
        infile >> index[0] >> index[1] >> index[2];
        Point<Real, 3> tp[3] = {vecP[--index[0]], vecP[--index[1]],
                                vecP[--index[2]]};
        Triangle<Real, 3> tri(tp);
        vectri.push_back(tri);
        size++;
      } else if (c == 'g')
        break;
    }
    GluingCompactSurface<Real> gcs(vectri);
    vector<GluingCompactSurface<Real>> vecgcs = {gcs};
    YinSet<Real> y(vecgcs);
    return y;
  }

  assert(false && "import file not find.");
  return YinSet<Real>();
}