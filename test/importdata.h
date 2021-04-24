#include <fstream>
#include <iostream>
#include <vector>
#include "../src/YinSet.h"

using namespace std;
using namespace YSB;

YinSet<Real> importdata(string s)
{
    ifstream infile(s);
    if (infile.is_open())
    {
        vector<Triangle<Real, 3>> vectri;
        vector<Point<Real, 3>> vecp;
        int size = 0;
        while (!infile.eof())
        {
            char c;
            int index[3];
            infile >> c;
            if (infile.fail())
            {
                break;
            }
            if (c == 'v')
            {
                Real p[3];
                infile >> p[0] >> p[1] >> p[2];
                vecp.emplace_back(p);
                size++;
            }
            else if (c == 'f')
            {
                infile >> index[0] >> index[1] >> index[2];
                Point<Real, 3> tp[3] = {vecp[--index[0]], vecp[--index[1]], vecp[--index[2]]};
                Triangle<Real, 3> tri(tp);
                vectri.push_back(tri);
                size++;
            }
        }
        GluingCompactSurface<Real> gcs(vectri);
        vector<GluingCompactSurface<Real>> vecgcs = {gcs};
        YinSet<Real> y(vecgcs);
        return y;
    }
}