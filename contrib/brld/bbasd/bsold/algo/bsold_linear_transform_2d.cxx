//:
// \file
// \author Nhon Trinh

#include "dbsol_linear_transform_2d.h"
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_curve_2d.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_polyline_2d.h>
#include <vsol/vsol_digital_curve_2d.h>
#include <vsol/vsol_conic_2d.h>
#include <vsol/vsol_region_2d.h>
#include <vsol/vsol_polygon_2d.h>
#include <vsol/vsol_group_2d.h>
#include <vsol/vsol_curve_2d_sptr.h>
#include <vsol/vsol_region_2d_sptr.h>

//: translate an vsol_spatial_object_2d with a vector
// return false if the vsol_2d object type is unknown to the function
bool dbsol_linear_transform_2d::
translate( const vsol_spatial_object_2d_sptr& a, const vgl_vector_2d< double >& v )
{
  // check type of object and call call proper functions. if none valid, return false.
  // point
  if (a->cast_to_point())
  {
    return translate(vsol_point_2d_sptr(a->cast_to_point()), v);
  }
  // curve
  if (a->cast_to_curve())
  {
    vsol_curve_2d_sptr a_curve(a->cast_to_curve());
    if (a_curve->cast_to_line())
      return translate(vsol_line_2d_sptr(a_curve->cast_to_line()), v);
    if (a_curve->cast_to_polyline())
      return translate(vsol_polyline_2d_sptr(a_curve->cast_to_polyline()), v);
    if (a_curve->cast_to_digital_curve())
      return translate(vsol_digital_curve_2d_sptr(a_curve->cast_to_digital_curve()), v);
    if (a_curve->cast_to_conic())
      return translate(vsol_conic_2d_sptr(a_curve->cast_to_conic()), v);
    return false;
  }

  // region
  if (a->cast_to_region())
  {
    vsol_region_2d_sptr a_region(a->cast_to_region());
    if (a_region->cast_to_polygon())
      return translate(vsol_polygon_2d_sptr(a_region->cast_to_polygon()), v);
    return false;
  }
  // group
  if (a->cast_to_group())
  {
    return translate(vsol_group_2d_sptr(a->cast_to_group()), v);
  }
    
  return false;
}


//: translate a point
bool dbsol_linear_transform_2d::
translate( const vsol_point_2d_sptr& p, const vgl_vector_2d< double >& v )
{
  p->add_vector(v);
  return true;
}

//: translate a line
bool dbsol_linear_transform_2d::
translate( const vsol_line_2d_sptr& line, const vgl_vector_2d< double >& v )
{
  line->p0()->add_vector(v);
  line->p1()->add_vector(v);
  return true;
}


//: translate a polyline
// This function assumes there is no repeated vertex in the polyline
bool dbsol_linear_transform_2d::
translate( const vsol_polyline_2d_sptr& polyline, const vgl_vector_2d< double >& v )
{
  if (polyline->size() < 2) 
    return false;
  for (unsigned int i = 0; i < polyline->size(); i ++ )
  {
    polyline->vertex(i)->add_vector(v);
  }
  return true;
}

//: translate a digital curve
// This function assumes there is no repeated vertex in the digital curve
bool dbsol_linear_transform_2d::
translate( const vsol_digital_curve_2d_sptr& digital_curve, const vgl_vector_2d< double >& v )
{
  if (digital_curve->size() < 2)
    return false;
  for (unsigned int i = 0; i < digital_curve->size(); i ++ )
  {
    digital_curve->point(i)->add_vector(v);
  }
  return true;
}

//: translate a conic
bool dbsol_linear_transform_2d::
translate( const vsol_conic_2d_sptr& conic, const vgl_vector_2d< double >& v )
{
  conic->translate_by(v.x(), v.y());
  return true;
}



//: translate a polygon
bool dbsol_linear_transform_2d::
translate( const vsol_polygon_2d_sptr& polygon, const vgl_vector_2d< double >& v )
{
  for (unsigned int i = 0; i < polygon->size(); i++)
  {
    polygon->vertex(i)->add_vector(v);  
  }
  return true;

}

//: translate a group
bool dbsol_linear_transform_2d::
translate( const vsol_group_2d_sptr& group, const vgl_vector_2d< double >& v )
{
  bool success = true;
  unsigned int i = 0;
  for (i = 0; i < group->size() && success; i ++ )
  {
    success = success && (dbsol_linear_transform_2d::translate(group->object(i), v));
  }
  if (success) 
    return true;
  // if one of the objects cannot be translated, undo all the translation done so far
  for (unsigned int j = 0; j < i; j ++ )
  {
    dbsol_linear_transform_2d::translate(group->object(j), -v);
  }
  return false;
}






