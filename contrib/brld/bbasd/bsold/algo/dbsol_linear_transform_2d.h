// This is basic/dbsol/algo/dbsol_linear_transform_2d.h
#ifndef dbsol_linear_transform_2d_h
#define dbsol_linear_transform_2d_h
//:
//\file
//\brief A class to perform linear transformation on vsol_2d objects (translation, rotation)
//\author Nhon Trinh (ntrinh@lems.brown.edu)
//\date April 11, 2005


#include <vgl/vgl_vector_2d.h>
#include <vsol/vsol_spatial_object_2d_sptr.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_line_2d_sptr.h>
#include <vsol/vsol_polyline_2d_sptr.h>
#include <vsol/vsol_digital_curve_2d_sptr.h>
#include <vsol/vsol_conic_2d_sptr.h>
#include <vsol/vsol_polygon_2d_sptr.h>
#include <vsol/vsol_group_2d_sptr.h>

//: A class to perform linear transformation on vsol_2d objects (translation, rotation)
// All public functions in this class are static and the constructor is disabled
// To perform operation on vsol_2d object v, use 
// dbsol_linear_transform.function(v, ...)
class dbsol_linear_transform_2d
{
private:
  //: disable constructor
  dbsol_linear_transform_2d();
public:
  //: destructor
  ~dbsol_linear_transform_2d();

  //: translate an vsol_spatial_object_2d with a vector
  // return false if the vsol_2d object type is unknown to the function
  static bool translate( const vsol_spatial_object_2d_sptr& a, const vgl_vector_2d< double >& v );
  //: translate a point
  static bool translate( const vsol_point_2d_sptr& p, const vgl_vector_2d< double >& v );
  //: translate a line
  static bool translate( const vsol_line_2d_sptr& line, const vgl_vector_2d< double >& v );
  //: translate a polyline
  static bool translate( const vsol_polyline_2d_sptr& polyline, const vgl_vector_2d< double >& v );
  //: translate a digital curve
  static bool translate( const vsol_digital_curve_2d_sptr & digital_curve, const vgl_vector_2d< double >& v );
  //: translate a conic
  static bool translate( const vsol_conic_2d_sptr& conic, const vgl_vector_2d< double >& v );
  //: translate a polygon
  static bool translate( const vsol_polygon_2d_sptr& polygon, const vgl_vector_2d< double >& v );
  //: translate a group
  static bool translate( const vsol_group_2d_sptr& group, const vgl_vector_2d< double >& v );
  

};

#endif // dbsol_linear_transform_2d_h
