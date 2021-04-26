#ifndef IMPORT_DATA1
#define IMPORT_DATA1

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "../src/YinSet.h"

using namespace std;
using namespace YSB;

YinSet<Real> importdata1(string s)
{
    ifstream infile(s);
    if (infile.is_open())
    {
        vector<Triangle<Real, 3>> vectri;
        vector<Point<Real, 3>> vecp;
        string ss, s1, s2, s3, s4;
        while (!infile.eof())
        {
            int vn = 0;
	        int vnum = 0;
	        int fnum = 0;
 
            while (getline(infile, ss))
            {
                istringstream in(ss);
                in >> s1 >> s2 >> s3 >> s4;
        
                if (ss[0]=='v')
                {
                    if (ss[1] == 'n')
                    {
                        vn++;
                    }
                    else if(ss[1] == 't')
                    {
                        ;
                    }
                    else
                    {
                        vnum++;
                        Real p[3];
                        p[0] = stod(s2);
                        p[1] = stod(s3);
                        p[2] = stod(s4);
                        vecp.emplace_back(p);
                    }
        
                }
        
                int a = 0, b = 0, c = 0;
                if (ss[0] == 'f')
                {
                    // if(vnum==2708)
                    // int asd = vnum;
                    for (int k = 0; s2[k] != '/'&& s2[k] != 0 ; k++)
                    {
                        a = a * 10 + (s2[k] - 48);
                    }
        
                    for (int k = 0; s3[k] != '/'&& s3[k] != 0; k++)
                    {
                        b = b * 10 + (s3[k] - 48);
                    }
        
                    for (int k = 0; s4[k] != '/'&& s4[k] != 0; k++)
                    {
                        c = c * 10 + (s4[k] - 48);
                    }
                    fnum++;
                    a--;
                    b--;
                    c--;
                    Point<Real, 3> tp[3] = {vecp[a], vecp[b], vecp[c]};
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
}


#endif