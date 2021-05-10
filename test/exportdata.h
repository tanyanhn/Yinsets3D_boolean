#include <sys/stat.h>
#include <sys/types.h>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../src/YinSet.h"

using namespace std;
using namespace YSB;

inline void exportdata(string s, YinSet<Real>& y, string folder) {
  const char* path = folder.c_str();
  int isCreate = mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
  if (!isCreate)
    printf("create path:%s\n", path);
  else
    printf("create path failed! error code : %d %s \n", isCreate, path);

  auto&& vecgcs = y.gcss();
  for (size_t i = 0; i < vecgcs.size(); i++) {
    ofstream outfile(folder + "/" + s + to_string(i) + ".obj");
    if (outfile.is_open()) {
      vector<Triangle<Real, 3>> vectri;
      vecgcs[i].collapse(vectri, 1, 1);
      set<Point<Real, 3>, PointCompare> setp;
      for (std::size_t i = 0; i < vectri.size(); i++) {
        setp.insert(vectri[i].vert(0));
        setp.insert(vectri[i].vert(1));
        setp.insert(vectri[i].vert(2));
      }
      for (auto&& p : setp) {
        outfile << "v"
                << " " << p[0] << " " << p[1] << " " << p[2] << endl;
      }
      for (size_t i = 0; i < vectri.size(); i++) {
        int index[3];
        auto itp0 = setp.find(vectri[i].vert(0));
        assert(itp0 != setp.end() && "point find error");
        index[0] = distance(setp.begin(), itp0);
        auto itp1 = setp.find(vectri[i].vert(1));
        assert(itp1 != setp.end() && "point find error");
        index[1] = distance(setp.begin(), itp1);
        auto itp2 = setp.find(vectri[i].vert(2));
        assert(itp2 != setp.end() && "point find error");
        index[2] = distance(setp.begin(), itp2);
        outfile << "f"
                << " " << ++index[0] << " " << ++index[1] << " " << ++index[2]
                << endl;
      }
    }
  }
}