#include<fstream>
#include<iostream>
#include<vector>
#include "../src/Yinset.h"

using namespace std;
using namespace YSB;

void exportdata(string s, YinSet<Real> y)
{
    ofstream outfile(s);
    if(outfile.is_open())
    {
        vector<Triangle<Real, 3>> vectri;
        y.collapse(vectri, 1);
        set<Point<Real, 3>, PointCompare> setp;
        for(int i = 0; i<vectri.size(); i++)
        {
            setp.insert(vectri[i].vert(0));
            setp.insert(vectri[i].vert(1));
            setp.insert(vectri[i].vert(2));
        }
        for(auto &&p : setp )
        {
            outfile<<'v'<<" "<<p[0]<<" "<<p[1]<<" "<<p[2]<<endl;
        }
        for(int i = 0; i<vectri.size(); i++)
        {
            int index[3];
            
        }
    }
}