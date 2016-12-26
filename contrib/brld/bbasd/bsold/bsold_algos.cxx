// This is dbsol/dbsol_algos.cxx
//:
// \file

#include "dbsol_algos.h"

//#include <vcl_cstring.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_polyline_2d.h>
#include <vgl/vgl_distance.h>
#include <vnl/vnl_random.h>
#include <vcl_cmath.h>

const double dbsol_algos_epsilon = 1e-10;


//: Break a polyline into two pieces, each is a polyline
vcl_vector<vsol_polyline_2d_sptr > dbsol_algos::
cut_polyline(const vsol_polyline_2d_sptr& polyline,
             const vsol_point_2d_sptr& cut_position)
{
  vcl_vector<vsol_polyline_2d_sptr > new_polys;

  // find order of the cutting_position vertex
  unsigned int index = 0;
  for (; index < polyline->size() && polyline->vertex(index) != cut_position; 
    ++index)
  {}

  if (index == polyline->size()) return new_polys;

  // if the cut_position is the first or last vertex, then just return the original polyline
  if (index ==0 || index == int(polyline->size())-1)
  {
    new_polys.push_back(polyline);
    return new_polys;
  }

  vcl_vector<vsol_point_2d_sptr > pt_set1;
  pt_set1.reserve(index+1);
  for (unsigned int i = 0; i<=index; ++i)
  {
    pt_set1.push_back(new vsol_point_2d(polyline->vertex(i)->get_p()));
  }
  new_polys.push_back(new vsol_polyline_2d(pt_set1));

  vcl_vector<vsol_point_2d_sptr > pt_set2;
  pt_set2.reserve(polyline->size()-index-1);
  for (unsigned int i = index; i < polyline->size(); ++i)
  {
    pt_set2.push_back(new vsol_point_2d(polyline->vertex(i)->get_p()));
  }
  new_polys.push_back(new vsol_polyline_2d(pt_set2));
  return new_polys;
}




// ----------------------------------------------------------------------------
vsol_polyline_2d_sptr dbsol_algos::
polyline_from_polygon(const vsol_polygon_2d_sptr& polygon,
                      const vsol_point_2d_sptr& cut_position)
{
  if (!polygon || polygon->size() <3) return 0;

  // find index of cutting position
  unsigned int index = 0;
  if (cut_position)
  {
    for (unsigned int i=0; i<polygon->size(); ++i)
    {
      if (cut_position == polygon->vertex(i))
      {
        index = i;
        break;
      }
    }
  }

  // construct a new polyline

  // first need to circular-shift the vertex set
  vcl_vector<vsol_point_2d_sptr > pt_set;
  pt_set.reserve(polygon->size());
  for (unsigned int i=index; i<polygon->size(); ++i)
  {
    pt_set.push_back(new vsol_point_2d(*polygon->vertex(i)));
  }

  for (unsigned int i=0; i<=index; ++i)
  {
    pt_set.push_back(new vsol_point_2d(*polygon->vertex(i)));
  }

  return new vsol_polyline_2d(pt_set);
}



vsol_polygon_2d_sptr dbsol_algos::
polygon_from_polyline(const vsol_polyline_2d_sptr& polyline)
{
  // construct a new polygon using the vertices of the polyline
  vcl_vector<vsol_point_2d_sptr > pt_set;
  pt_set.reserve(polyline->size());
  for (unsigned int i=0; i < polyline->size(); ++i)
  {
    pt_set.push_back(new vsol_point_2d(*polyline->vertex(i)));
  }

  // check distance between the two end points. 
  // If they're too close, delete one to avoid duplicate points in the polygon
  if (pt_set.front()->distance(pt_set.back()) < dbsol_algos_epsilon)
  {
    pt_set.pop_back();
  }

  return new vsol_polygon_2d(pt_set);
}



// ----------------------------------------------------------------------------
//: find the closest point on a polyline
vsol_point_2d_sptr dbsol_algos::
closest_point(const vsol_point_2d_sptr& p,
              const vsol_polyline_2d_sptr& polyline,
              double & d)
{
  if (!polyline) return 0;
  if (polyline->size()==0) return 0;

  double min_dist = p->distance(polyline->vertex(0));
  vsol_point_2d_sptr min_pt = polyline->vertex(0);

  for (unsigned int i=1; i<polyline->size(); ++i)
  {
    double new_d = p->distance(polyline->vertex(i));
    if (new_d < min_dist)
    {
      min_dist = new_d;
      min_pt = polyline->vertex(i);
    }
  }

  d = min_dist;
  return min_pt;
}


