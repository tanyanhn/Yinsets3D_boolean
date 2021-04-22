#include "../catch.hpp"
#include "../src/PrePast.h"
#include "../src/Past.h"
#include <vector>

using namespace YSB;
using namespace std;

TEST_CASE("Paste", "[pa1]")
{
    Real r1[3] = {0.0, -1.0, 0.0}, r2[3] = {1.0, 0.0, 0.0}, r3[3] = {-1.0, 0.0, 0.0}, r4[3] = {0.0, 1.0, 0.0};
    Real r5[3] = {0.0, 0.0, 3.0};
    Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5);
    Point<Real, 3> tripoints1[] = {tp1, tp2, tp5}, tripoints2[] = {tp2, tp4, tp5},
                   tripoints3[] = {tp2, tp1, tp3}, tripoints4[] = {tp1, tp5, tp3},
                   tripoints5[] = {tp5, tp4, tp3}, tripoints6[] = {tp2, tp3, tp4};
    Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3), tri4(tripoints4), tri5(tripoints5), tri6(tripoints6);
    vector<Triangle<Real, 3>> triA{tri1, tri2, tri3, tri4, tri5, tri6}, triB;
    for (int i = 0; i < triA.size(); i++)
    {
        for (int k = 0; k < 3; k++)
        {
            triA[i].ed(k).neighborhood().push_back(make_pair(1, i));
            for (int j = 0; j < triA.size(); j++)
            {
                if (j != i)
                {
                    for (int h = 0; h < 3; h++)
                    {
                        if (norm(triA[j].ed(h)[0] - triA[i].ed(k)[0]) < TOL && norm(triA[j].ed(h)[1] - triA[i].ed(k)[1]) < TOL)
                            triA[i].ed(k).neighborhood().push_back(make_pair(1, j));
                        if (norm(triA[j].ed(h)[0] - triA[i].ed(k)[1]) < TOL && norm(triA[j].ed(h)[1] - triA[i].ed(k)[0]) < TOL)
                            triA[i].ed(k).neighborhood().push_back(make_pair(1, j));
                    }
                }
            }
        }
    }
    triA[0].ed(1).IntersectionSeg() = 1;
    triA[1].ed(2).IntersectionSeg() = 1;
    triA[2].ed(2).IntersectionSeg() = 1;
    triA[3].ed(1).IntersectionSeg() = 1;
    triA[4].ed(2).IntersectionSeg() = 1;
    triA[5].ed(0).IntersectionSeg() = 1; 
    PrePast<Real> pp;
    pp(triA, 1);
    vector<SurfacePatch<Real>> vSP1 = pp.vecSPA, vSP2, vecF;
    Past<Real> pa;
    pa.combine(vSP1, vSP2, triA, triB, vecF);
    pa(vecF, triA, triB);  
    vector<GluingCompactSurface<Real>> vecGCS = pa.vecGCS; 
   
    REQUIRE(vecGCS.size() == 1);
    REQUIRE(vecGCS[0].tris().size() == 6);
    
}
