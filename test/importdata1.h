#ifndef IMPORT_DATA1
#define IMPORT_DATA1

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "../src/YinSet.h"

using namespace std;
using namespace YSB;

inline YinSet<Real> importdata1(string s) {
  ifstream infile(s);
  if (infile.is_open()) {
    vector<Triangle<Real, 3>> vectri;
    vector<Point<Real, 3>> vecP;
    string ss, s1, s2, s3, s4;
    while (!infile.eof()) {
      int vn = 0;
      int vNum = 0;
      int fNum = 0;

      while (getline(infile, ss)) {
        istringstream in(ss);
        in >> s1 >> s2 >> s3 >> s4;

        if (ss[0] == 'v') {
          if (ss[1] == 'n') {
            vn++;
          } else if (ss[1] == 't') {
            ;
          } else {
            vNum++;
            Real p[3];
            p[0] = stod(s2);
            p[1] = stod(s3);
            p[2] = stod(s4);
            vecP.emplace_back(p);
          }
        }

        int a = 0, b = 0, c = 0;
        if (ss[0] == 'f') {
          for (int k = 0; s2[k] != '/' && s2[k] != 0; k++) {
            a = a * 10 + (s2[k] - 48);
          }

          for (int k = 0; s3[k] != '/' && s3[k] != 0; k++) {
            b = b * 10 + (s3[k] - 48);
          }

          for (int k = 0; s4[k] != '/' && s4[k] != 0; k++) {
            c = c * 10 + (s4[k] - 48);
          }
          fNum++;
          a--;
          b--;
          c--;
          Point<Real, 3> tp[3] = {vecP[a], vecP[b], vecP[c]};
          Triangle<Real, 3> tri(tp);
          vectri.push_back(tri);
        }
      }
      infile.close();
      GluingCompactSurface<Real> gcs(vectri);
      vector<GluingCompactSurface<Real>> vecgcs = {gcs};
      YinSet<Real> y(vecgcs);
      return y;
    }
  }
  assert(false && "import file not find.");
  return YinSet<Real>();
}

#endif