#ifndef EXPORTDATA_INNER_H
#define EXPORTDATA_INNER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include "YinSet.h"

namespace YSB
{

    void exportdata_inner(std::string s, YinSet<Real> &y, std::string folder)
    {
        const char *path = folder.c_str();
        int isCreate = mkdir(path, S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO);
        if (!isCreate)
            printf("create path:%s\n", path);
        else
            printf("create path failed! error code : %d %s \n", isCreate, path);

        auto &&vecgcs = y.gcss();
        for (int i = 0; i < vecgcs.size(); i++)
        {
            std::ofstream outfile(folder + "/" + s + std::to_string(i) + ".obj");
            if (outfile.is_open())
            {
                std::vector<Triangle<Real, 3>> vectri;
                vecgcs[i].collapse(vectri, 1, 1);
                std::set<Point<Real, 3>, PointCompare> setp;
                for (int i = 0; i < vectri.size(); i++)
                {
                    setp.insert(vectri[i].vert(0));
                    setp.insert(vectri[i].vert(1));
                    setp.insert(vectri[i].vert(2));
                }
                for (auto &&p : setp)
                {
                    outfile << "v"
                            << " " << p[0] << " " << p[1] << " " << p[2] << std::endl;
                }
                for (int i = 0; i < vectri.size(); i++)
                {
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
                            << " " << ++index[0] << " " << ++index[1] << " " << ++index[2] << std::endl;
                }
            }
        }
    }

} // namespace YSB

#endif // !E