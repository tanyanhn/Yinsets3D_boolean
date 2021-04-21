#include "../catch.hpp"
#include "../src/RemoveOverlap.h"
#include <vector>

using namespace YSB;
using namespace std;

TEST_CASE("Prepaste", "[pp1]")
{
    Real r1[3] = {0.0, -1.0, 0.0}, r2[3] = {-1.0, 0.0, 0.0}, r3[3] = {1.0, 0.0, 0.0}, r4[3] = {0.0, 1.0, 0.0};
    Real r5[3] = {0.0, 0.0, 3.0}, r6[3] = {0.0, 0.0, -3.0};
    Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6);
    Point<Real, 3> tripoints1[] = {tp1, tp3, tp5}, tripoints2[] = {tp3, tp4, tp5},
                   tripoints3[] = {tp3, tp1, tp2}, tripoints4[] = {tp1, tp5, tp2},
                   tripoints5[] = {tp5, tp4, tp2}, tripoints6[] = {tp3, tp2, tp4},
                   tripoints7[] = {tp3, tp1, tp2}, tripoints8[] = {tp3, tp2, tp4},
                   tripoints9[] = {tp3, tp4, tp6};
    Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3),
                      tri4(tripoints4), tri5(tripoints5), tri6(tripoints6), 
                      tri7(tripoints7), tri8(tripoints8), tri9(tripoints9);
    vector<Triangle<Real, 3>> triA{tri1, tri2, tri3, tri4, tri5, tri6}, triB{tri7, tri8, tri9};
    std::map<int, std::pair<std::vector<Segment<Real, 3>>, std::vector<std::pair<int, int>>>> resultA, resultB;
    resultA[2].second.push_back({2, 0});
    resultA[5].second.push_back({2, 1});
    resultB[0].second.push_back({1, 2});
    resultB[1].second.push_back({1, 5});
  
    std::map<int, std::vector<int>> TriangulateA, TriangulateB;


    for (int i = 0; i < triA.size(); i++)
    {
        TriangulateA[i].push_back(i);
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
    for (int i = 0; i < triB.size(); i++)
    {
        TriangulateB[i].push_back(i);
        for (int k = 0; k < 3; k++)
        {
            triB[i].ed(k).neighborhood().push_back(make_pair(2, i));
            for (int j = 0; j < triB.size(); j++)
            {
                if (j != i)
                {
                    for (int h = 0; h < 3; h++)
                    {
                        if (norm(triB[j].ed(h)[0] - triB[i].ed(k)[0]) < TOL && norm(triB[j].ed(h)[1] - triB[i].ed(k)[1]) < TOL)
                            triB[i].ed(k).neighborhood().push_back(make_pair(2, j));
                        if (norm(triB[j].ed(h)[0] - triB[i].ed(k)[1]) < TOL && norm(triB[j].ed(h)[1] - triB[i].ed(k)[0]) < TOL)
                            triB[i].ed(k).neighborhood().push_back(make_pair(2, j));
                    }
                }
            }
        }
    }

    cout<<resultB.size();
    cout<<resultA.size();
    RemoveOverlap<Real> rp;
    rp(TriangulateA, TriangulateB, triA, triB, resultA, resultB);
    REQUIRE(triB[0].IfRemoved()==true);
    //REQUIRE(triB[1].ed(2).neighborhood().size()==1);
    REQUIRE(triB[1].IfRemoved()==true);
    REQUIRE(triA[2].IfRemoved()==false); 
    REQUIRE(triA[5].IfRemoved()==false); 
}
