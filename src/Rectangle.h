#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <set>
#include "Config.h"
#include "Core/Interval.h"
#include "Plane.h"
#include "PointCompare.h"
#include "Triangle.h"
#include "SegmentCompare.h"

namespace YSB{

template<class T, int Dim>
class Rectangle
{
 public:
   Rectangle(){}
   Rectangle(const Point<T,Dim> &p1, const Point<T,Dim> &p2,
            const Point<T,Dim> &p3, const Point<T,Dim> &p4):vertex{p1,p2,p3,p4}
            {
               edge[0] = Segment<T, Dim>(vertex[0], vertex[1]);
               edge[1] = Segment<T, Dim>(vertex[1], vertex[2]);
               edge[2] = Segment<T, Dim>(vertex[2], vertex[3]);
               edge[3] = Segment<T, Dim>(vertex[3], vertex[0]);
               pla = new Plane<T>(p1, cross(p2-p1, p3-p1));
               pla->normVec = normalize(pla->normVec) * perimeter();    
            }


   Real perimeter() const {
      return norm(vertex[0] - vertex[1]) + norm(vertex[1] - vertex[2]) +
         norm(vertex[2] - vertex[3]) + norm(vertex[3] - vertex[0]);
   }

   Point<T, Dim>& vert(int i) { return vertex[i]; }
   const Point<T, Dim>& vert(int i) const { return vertex[i]; }

   Segment<T, Dim>& ed(int i) { return edge[i]; }
   const Segment<T, Dim>& ed(int i) const { return edge[i]; }

   int majorDim(int k = 1) const {
      return pla->majorDim(k);
   }

   Rectangle<T, Dim-1> project(int d)
   {
      if (d == -1) {
         d = majorDim();
         }
         Point<T, Dim - 1> v[4];
         v[0] = vertex[0].project(d);
         v[1] = vertex[1].project(d);
         v[2] = vertex[2].project(d);
         v[3] = vertex[3].project(d);
         return Rectangle<T, Dim - 1>(v);
   }

   int intersect(const Triangle<T, Dim> &tri, 
                           std::vector<Segment<T, Dim>>& result, 
                           Real tol = TOL) const{

      Point<T, Dim> vecp1[] = {vertex[0], vertex[1], vertex[2]};
      Point<T, Dim> vecp2[] = {vertex[2], vertex[3], vertex[0]};
      Triangle<T, Dim> tri1(vecp1), tri2(vecp2);

      tri.intersect(tri1, result, tol);
      tri.intersect(tri2, result, tol);

      return result.size();

      

                              
   }

   int intersect(const Segment<T, Dim>& seg, 
                           std::vector<Segment<T, Dim-1>>& result, 
                           Real tol = TOL) const{
      int mDim = majorDim();
      Rectangle<T, Dim-1> projrect = project(mDim);
      Segment<T, Dim-1> projseg = seg.project(mDim);
      Line<T, Dim-1> projl(projseg[0], projseg[1]-projseg[0]);

      std::set<Point<T,Dim-1>, PointCompare> setp;
      std::vector<Point<T,Dim-1>> resultp;

      for (auto i = 0; i < 4; ++i) {
         intersectSegLine(projrect.edge[i], projl, resultp, tol);
      }

      for (auto&& ip : resultp) {
         setp.insert(ip);
      }
      resultp.clear();
      for (auto&& ip : setp) {
         resultp.push_back(ip);
      }

      if(resultp.size() == 0)
      return 0;
      else if(resultp.size() == 1)
      {
         resultp.push_back(resultp[0]);
      }

      resultp.push_back(projseg[0], projseg[1]);
      std::vector<Point<T, Dim-1>> rsEndp;
      typename Segment<T, Dim-1>::intsType segintsT = YSB::solveForOverlie(
         resultp[0], resultp[1], resultp[2], resultp[3], rsEndp);

      if (segintsT == Segment<T, Dim-1>::One) {
         result.emplace_back(Segment<T, Dim-1>(rsEndp[0], rsEndp[0]));
      } else if (segintsT == Segment<T, Dim-1>::Overlap) {
         result.emplace_back(Segment<T, Dim-1>(rsEndp[0], rsEndp[1]));
      }

      return result.size();


   }
      




 private:
    Point<T, Dim> vertex[4];
    Segment<T, Dim> edge[4];
    mutable Plane<T>* pla;
};

}






#endif