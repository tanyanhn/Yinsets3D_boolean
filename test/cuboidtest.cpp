#include "../catch.hpp"
#include "../src/Cuboid.h"
#include "../src/Spacedivision.h"

using namespace YSB;
using namespace std;

TEST_CASE("cuboid1", "[cub1]") {
  Real r1[3] = {0.0, 0.0, 0.0}, r2[3] = {1.0, 0.0, 0.0},
       r3[3] = {1.0, 1.0, 0.0}, r4[3] = {0.0, 1.0, 0.0},
       r5[3] = {0.0, 0.0, 1.0}, r6[3] = {1.0, 0.0, 1.0},
       r7[3] = {1.0, 1.0, 1.0}, r8[3] = {0.0, 1.0, 1.0},
       r9[3] = {0.75, 0.25, 0.0}, r10[3] = {0.25, 0.75, 0.0},
       r11[3] = {0.5, 0.5, 1.0}, r12[3] = {0.75, 0.75, 0.0},
       r13[3] = {0.5, 0.75, 1};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6), tp7(r7), tp8(r8), tp9(r9),
                    tp10(r10), tp11(r11), tp12(r12), tp13(r13);

  Point<Real, 3> tripoints1[] = {tp11, tp9, tp10}, tripoint2[] = {tp10, tp12, tp13},
                  cubpoints[] = {tp1, tp2, tp3, tp4, tp5, tp6, tp7, tp8};
  Triangle<Real, 3> tri1(tripoints1), tri2(tripoint2);
  Cuboid<Real> cub(cubpoints);
  vector<Cuboid<Real>> subcubs = cub.divide();
  for(int i=0;i<subcubs.size();i++)
  {
      cout<<"The "<<i<<"th subcub:"<<endl;
      for(int j=0;j<8;j++)
      {
          cout<<subcubs[i].vert(j)<<" ";
      }
      cout<<endl<<endl;
  }
  REQUIRE(subcubs.size() == 8);

  SpaceDivision<Real> spd;
  vector<int> res1 = spd.intersectDividedCuboid(tri1, cub);
  int count1 = 0; 
  for(int i=0;i<8;i++)
  count1 += res1[i];
  
  REQUIRE(count1==8);

  vector<int> res2 = spd.intersectDividedCuboid(tri2, cub);
  int count2 = 0;
  for(int i=0;i<8;i++)
  count2 += res2[i];
  
  REQUIRE(count2==4);


}


TEST_CASE("innerintersect", "[iins1]") {
  Real r1[3] = {0.0, 0.0, 0.0}, r2[3] = {1.0, 0.0, 0.0},
       r3[3] = {1.0, 1.0, 0.0}, r4[3] = {0.0, 1.0, 0.0},
       r5[3] = {0.0, 0.0, 1.0}, r6[3] = {1.0, 0.0, 1.0},
       r7[3] = {1.0, 1.0, 1.0}, r8[3] = {0.0, 1.0, 1.0},
       r9[3] = {0.75, 0.25, 0.0}, r10[3] = {0.25, 0.75, 0.0},
       r11[3] = {0.5, 0.5, 1.0}, r12[3] = {0.75, 0.75, 0.0},
       r13[3] = {0.5, 0.75, 1};
  Point<Real, 3> tp1(r1), tp2(r2), tp3(r3), tp4(r4), tp5(r5), tp6(r6), tp7(r7), tp8(r8), tp9(r9),
                    tp10(r10), tp11(r11), tp12(r12), tp13(r13);

  Point<Real, 3> tripoints1[] = {tp1, tp2, tp3}, tripoints2[] = {tp1, tp2, tp5},
                  tripoints3[] = {tp1, tp2, tp7};
  Triangle<Real, 3> tri1(tripoints1), tri2(tripoints2), tri3(tripoints3);
  vector<Triangle<Real, 3>> vectri = {tri1, tri2, tri3};
  SpaceDivision<Real> spd;
  auto res = spd.intersectinner(vectri);
  
  REQUIRE(res.size()==1);


}