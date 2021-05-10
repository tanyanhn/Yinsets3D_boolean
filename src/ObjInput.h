#ifndef OBJINPUT_H
#define OBJINPUT_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "YinSet.h"

namespace YSB {
template <class T>
class YinSet;

inline YinSet<Real> objInput(const std::string& s, Real tol = TOL) {
  std::ifstream infile(s);
  PointCompare pCmp(tol);
  if (infile.is_open()) {
    std::vector<Triangle<Real, 3>> vectri;
    std::vector<Point<Real, 3>> vecP;
    std::vector<GluingCompactSurface<Real>> vecgcs;
    std::string ss, s1, s2, s3, s4;
    if (s.back() == 'j') {
      while (!infile.eof()) {
        int vn = 0;
        int vNum = 0;
        int fNum = 0;

        while (getline(infile, ss)) {
          std::istringstream in(ss);
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
            // if (pCmp.compare(tp[0], tp[1]) != 0 &&
            //     pCmp.compare(tp[1], tp[2]) != 0 &&
            //     pCmp.compare(tp[2], tp[0]) != 0)
            // {
            Triangle<Real, 3> tri(tp);
            vectri.push_back(tri);
            // }
          }

          if (ss[0] == 'o') {
            if (!vectri.empty()) {
              vecgcs.emplace_back(vectri);
              vectri.clear();
            }
          }
        }
        if (!vectri.empty()) {
          vecgcs.emplace_back(vectri);
          vectri.clear();
        }

        infile.close();
        return YinSet<Real>(vecgcs, 0);
      }
    } else if (s.back() == 'y') {
      while (!infile.eof()) {
        // int vn = 0;
        int vNum = 0;
        int fNum = 0;
        int header = 0;

        while (getline(infile, ss)) {
          std::istringstream in(ss);
          in >> s1 >> s2 >> s3 >> s4;

          if (header == 0) {
            if (ss.substr(0, 10) != "end_header")
              continue;
            else {
              s1.clear(), s2.clear(), s3.clear(), s4.clear();
              header = 1;
              continue;
            }
          } else if (s4.empty()) {
            vNum++;
            Real p[3];
            p[0] = stod(s1);
            p[1] = stod(s2);
            p[2] = stod(s3);
            vecP.emplace_back(p);
          } else if (ss[0] == '3') {
            int a = 0, b = 0, c = 0;
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
            Point<Real, 3> tp[3] = {vecP[a], vecP[b], vecP[c]};
            if (pCmp.compare(tp[0], tp[1]) != 0 &&
                pCmp.compare(tp[1], tp[2]) != 0 &&
                pCmp.compare(tp[2], tp[0]) != 0) {
              Triangle<Real, 3> tri(tp);
              vectri.push_back(tri);
            }
          }
        }
        if (!vectri.empty()) {
          vecgcs.emplace_back(vectri);
          vectri.clear();
        }
        infile.close();
        assert(!vecgcs.empty() && "input file not import rightly.");

        return YinSet<Real>(vecgcs, 0, tol);
      }
    }
  }
  return YinSet<Real>(std::vector<GluingCompactSurface<Real>>(), 0, tol);
}

}  // namespace YSB

#endif  // !OBJINPUT
