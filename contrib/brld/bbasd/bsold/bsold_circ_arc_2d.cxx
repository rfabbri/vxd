// This is basic/dbsol/vis/dbsol_circ_arc_2d.cxx
#include "dbsol_circ_arc_2d.h"

#include <vcl_cassert.h>
#include <vcl_cmath.h>
#include <vcl_iostream.h>
#include <vnl/vnl_math.h>
#include <vbl/io/vbl_io_smart_ptr.h>
#include <vsol/vsol_point_2d.h>
#include <vgl/vgl_homg_point_2d.h>
#include <dbgl/algo/dbgl_circ_arc.h>
#include <vgl/vgl_vector_2d.h>
#include <vgl/vgl_homg_line_2d.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/algo/vgl_homg_operators_2d.h>
#include <dbgl/algo/dbgl_closest_point.h>

//: Constructor - from starting point (P1), circular center (O)
// and normal vector at the ending point.
dbsol_circ_arc_2d::dbsol_circ_arc_2d( vsol_point_2d const& P1, 
                   vsol_point_2d const& O, 
                   vgl_vector_2d<double >const& end_normal_vector)
{
  set_from(P1.get_p(),O.get_p(),end_normal_vector);
}


//: Constructor - from 3 points
dbsol_circ_arc_2d::dbsol_circ_arc_2d(vsol_point_2d const& p1, 
                   vsol_point_2d const& p2, 
                   vsol_point_2d const& end_point)
{
  set_from(p1.get_p(),p2.get_p(),end_point.get_p());

}

//---------------------------------------------------------------------------
//: Clone `this': creation of a new object and initialization
// See Prototype pattern
//---------------------------------------------------------------------------
vsol_spatial_object_2d* dbsol_circ_arc_2d::clone() const
{
  return new dbsol_circ_arc_2d(*this);
}

//***************************************************************************
// Status setting
//***************************************************************************

//---------------------------------------------------------------------------
//: Set the first point of the curve
// Require: in(new_p0)
//---------------------------------------------------------------------------
void dbsol_circ_arc_2d::set_p0(vsol_point_2d_sptr const& new_p0)
{
  // require
  //assert(in(new_p0));

  p0_=new_p0;
}

//---------------------------------------------------------------------------
//: Set the last point of the curve
// Require: in(new_p1)
//---------------------------------------------------------------------------
void dbsol_circ_arc_2d::set_p1(vsol_point_2d_sptr const& new_p1)
{
  // require
  //assert(in(new_p1));

  p1_=new_p1;
}

//---------------------------------------------------------------------------
//: Return the length of `this'
//---------------------------------------------------------------------------
double dbsol_circ_arc_2d::length() const
{
  return this->len();
}


//--------------------------------------------------------------------
//: compute a dbgl arc corresponding to *this
//--------------------------------------------------------------------
dbgl_circ_arc dbsol_circ_arc_2d::dbgl_arc() const
{
  vsol_point_2d_sptr pnt0= this->p0();
  vsol_point_2d_sptr pnt1= this->p1();
  dbgl_circ_arc arc(pnt0->get_p(), pnt1->get_p(), this->curvature());
  return arc;
}



//  ***************************************************************************
//   Basic operations
//  ***************************************************************************

//---------------------------------------------------------------------------
//: Return the centre of the circle containing the arc
//---------------------------------------------------------------------------
vsol_point_2d_sptr dbsol_circ_arc_2d :: arc_center() const

{
  return new vsol_point_2d(this->center());
}

//---------------------------------------------------------------------------
//: Return the the curvature of the arc
//---------------------------------------------------------------------------
double dbsol_circ_arc_2d :: curvature() const
{
  return this->k();
}

//---------------------------------------------------------------------------
//: Is `p' in `this' ? (ie `p' verifies the equation, within some margin)
//---------------------------------------------------------------------------

int dbsol_circ_arc_2d::in(vsol_point_2d_sptr const& p) const
{
  return this->circ_region(vgl_point_2d<double>(p->x(),p->y()));
}

//---------------------------------------------------------------------------
//: Get tangent of the point at arclength s away from starting point
//---------------------------------------------------------------------------

vgl_vector_2d<double> dbsol_circ_arc_2d::
tangent(double s) const
{
  vgl_vector_2d<double > t = this->tangent_at_length(s);
  return t;
}

//---------------------------------------------------------------------------
//: Return the shortest distance of the point to the arc boundary
//---------------------------------------------------------------------------
double dbsol_circ_arc_2d::distance(vsol_point_2d_sptr const& query_pt) const
{
  return query_pt->distance(*(this->closest_point_on_curve(query_pt))); 
}

////---------------------------------------------------------------------------
////: Return the set of (real) intersection points of this arc with a line
////---------------------------------------------------------------------------
//vcl_list<vsol_point_2d_sptr>
//dbsol_circ_arc_2d::intersection(vsol_line_2d const& l) const
//{
//  vgl_homg_point_2d<double> p0(l.p0()->x(), l.p0()->y(), 1.0),
//                            p1(l.p1()->x(), l.p1()->y(), 1.0);
//  vgl_homg_line_2d<double> line(p0,p1);
//  vcl_list<vgl_homg_point_2d<double> > vv =
//    vgl_homg_operators_2d<double>::intersection(*this,line);
//  vcl_list<vsol_point_2d_sptr> v;
//  vcl_list<vgl_homg_point_2d<double> >::iterator it = vv.begin();
//  for (; !(it == vv.end()); ++it) {
//    if ((*it).w() != 0)  v.push_back(new vsol_point_2d((*it)));
//  }
//  return v;
//}

//---------------------------------------------------------------------------
//: Return the point on the arc boundary which is closest to the given point
//---------------------------------------------------------------------------
vsol_point_2d_sptr
dbsol_circ_arc_2d::closest_point_on_curve(vsol_point_2d_sptr const& pt) const
{
 double ret_ratio;
 double length_at_query;
 ret_ratio=dbgl_closest_point::point_to_circular_arc(pt->get_p(),p0_->get_p(),p1_->get_p(),this->k(),ret_ratio);
 length_at_query= ret_ratio * this->length();
 return new vsol_point_2d(vsol_point_2d(point_at_length(length_at_query)));
}
