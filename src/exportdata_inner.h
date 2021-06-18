#ifndef EXPORTDATA_INNER_H
#define EXPORTDATA_INNER_H

#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "YinSet.h"

namespace YSB {

inline void exportdata_inner(std::string s,
                             YinSet<Real>& y,
                             std::string folder,
                             int prec) {
  const char* path = folder.c_str();
  //int isCreate = mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
  int isCreate = mkdir(path);
  if (!isCreate)
    printf("create path:%s\n", path);
  else
    printf("create path failed! error code : %d %s \n", isCreate, path);

  auto&& vecgcs = y.gcss();
  int presize = 0;
  PointCompare pCmp(TOL / 1000);
  std::ofstream outfile(folder + "/" + s + ".obj");
  outfile.setf(std::ios::fixed, std::ios::floatfield);
  outfile.precision(prec);
  for (size_t j = 0; j < vecgcs.size(); j++) {
    outfile << "o  vecgcs." << std::to_string(j) << "\n";
    std::vector<Triangle<Real, 3>> vectri;
    std::set<Point<Real, 3>, PointCompare> setp(pCmp);
    std::set<Point<Real, 3>, PointCompare>::iterator wa;
    if (outfile.is_open()) {
      vecgcs[j].collapse(vectri, 1, 1);
      for (size_t i = 0; i < vectri.size(); i++) {
        setp.insert(vectri[i].vert(0));
        setp.insert(vectri[i].vert(1));
        setp.insert(vectri[i].vert(2));
      }
      for (auto&& p : setp) {
        outfile << "v"
                << " " << p[0] << " " << p[1] << " " << p[2] << std::endl;
      }
      for (size_t i = 0; i < vectri.size(); i++) {
        int index[3];
        if (j == 1 && i == 1026) {
          wa = std::next(setp.begin(), 1180);
        }
        auto itp0 = setp.find(vectri[i].vert(0));
        assert(itp0 != setp.end() && "point find error");
        index[0] = distance(setp.begin(), itp0) + presize;
        auto itp1 = setp.find(vectri[i].vert(1));
        assert(itp1 != setp.end() && "point find error");
        index[1] = distance(setp.begin(), itp1) + presize;
        auto itp2 = setp.find(vectri[i].vert(2));
        assert(itp2 != setp.end() && "point find error");
        index[2] = distance(setp.begin(), itp2) + presize;
        outfile << "f"
                << " " << ++index[0] << " " << ++index[1] << " " << ++index[2]
                << std::endl;
      }

      presize += setp.size();
    }
  }
}

}  // namespace YSB

#endif  // !E