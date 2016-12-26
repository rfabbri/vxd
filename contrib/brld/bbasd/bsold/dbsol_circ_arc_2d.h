// This is basic/dbsol/vis/dbsol_circ_arc_2d.h
#ifndef dbsol_circ_arc_2d_h
#define dbsol_circ_arc_2d_h
//:
// \file
// \brief  circular arc in a 2D space
// \author Isa Restrepo (mir@lems.brwon.edu)
// \date 4/9/2005
//
// \verbatim
//  Modifications
//    2006/08/17 Isa Restrepo   Creation
// \endverbatim
/********************************************/
//#include <bvis1/bvis1_tool.h>
//#include <bvis1/tool/bvis1_vsol2D_tools.h>

#include <dbgl/algo/dbgl_circ_arc.h>
#include <vgl/vgl_fwd.h>
#include <vgl/vgl_conic_segment_2d.h>
#include <vsl/vsl_binary_io.h>
#include <vsol/vsol_curve_2d.h>
#include <vsol/vsol_point_2d.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_line_2d.h>
#include <vsol/vsol_line_2d_sptr.h>
#include <dbsol/dbsol_circ_arc_2d_sptr.h>
#include <vnl/vnl_double_3x3.h>
#include <vcl_list.h>
#include <vcl_iostream.h>

class dbsol_circ_arc_2d : public vsol_curve_2d, public dbgl_circ_arc
{

  //***************************************************************************
  // Data members
  //***************************************************************************

  // Description: First point of the arc segment
  vsol_point_2d_sptr p0_;


  // Description: Last point of the arc segment
  vsol_point_2d_sptr p1_;

public:

  //**************************************************************
  // CONSTRUCTORS / DESTRUCTORS / INITIALIZERS
  //**************************************************************

  //: Constructor - default
  dbsol_circ_arc_2d(): vsol_curve_2d(), dbgl_circ_arc() {}

  //: Constructor - from intrinsic parameters;p1, p2 and curvature.
  dbsol_circ_arc_2d(vsol_point_2d const& p1 ,vsol_point_2d const& p2, double c ): vsol_curve_2d(), dbgl_circ_arc(p1.get_p(),p2.get_p(),c){}

  //: Constructor - from starting point (p1), circular center (o)
  // and normal vector at the ending point.
  dbsol_circ_arc_2d( vsol_point_2d const& P1, 
    vsol_point_2d const& O,  vgl_vector_2d<double >const& end_normal_vector);

  //: Constructor - from 3 points
  dbsol_circ_arc_2d(vsol_point_2d const& p1, 
    vsol_point_2d const& p2,vsol_point_2d const& end_point );


  //: Constructor from dbgl_circ_arc

  dbsol_circ_arc_2d(dbgl_circ_arc arc) : vsol_curve_2d(), dbgl_circ_arc(arc.point1(),arc.point2(),arc.k()),
    p0_(new vsol_point_2d(arc.point1())), p1_(new vsol_point_2d(arc.point2())){}

  //---------------------------------------------------------------------------
  //: Copy constructor
  //---------------------------------------------------------------------------
  dbsol_circ_arc_2d(dbsol_circ_arc_2d const& c):vsol_curve_2d(c),dbgl_circ_arc(c) {}


  //: Destructor
  ~dbsol_circ_arc_2d(){}

private: // has been superceeded by is_a()
  //: Return the curve type
  virtual vsol_curve_2d_type curve_type() const { return vsol_curve_2d::CONIC; }


public:

  //  : Clone `this': creation of a new object and initialization

  virtual vsol_spatial_object_2d* clone() const;

  //   Returns vsol_point_2d_sptr of the vsol_spatial_object_2d_sptr
  vsol_point_2d_sptr sptr() const;


  //****************************************************************************
  //Access
  //****************************************************************************

  //---------------------------------------------------------------------------
  //: Return the first point of `this';  pure virtual of vsol_curve_2d
  //---------------------------------------------------------------------------
  virtual vsol_point_2d_sptr p0() const { return p0_; }

  //---------------------------------------------------------------------------
  //  : Return the last point of `this';  pure virtual of vsol_curve_2d
  //  ---------------------------------------------------------------------------
  virtual vsol_point_2d_sptr p1() const { return p1_; }

  //***************************************************************************
  // Status report
  //***************************************************************************

  //---------------------------------------------------------------------------
  //: Return `this' if `this' is a circular arc, 0 otherwise
  //---------------------------------------------------------------------------
  virtual dbsol_circ_arc_2d const*cast_to_circ_arc()const{return this;}
  virtual dbsol_circ_arc_2d *cast_to_circ_arc() {return this;}



  //***************************************************************************
  // Status setting
  //***************************************************************************

  //---------------------------------------------------------------------------
  //: Set the first point of the curve
  //  REQUIRE: in(new_p0)
  //---------------------------------------------------------------------------
  virtual void set_p0(vsol_point_2d_sptr const& new_p0);

  //---------------------------------------------------------------------------
  //: Set the last point of the curve
  //  REQUIRE: in(new_p1)
  //---------------------------------------------------------------------------
  virtual void set_p1(vsol_point_2d_sptr const& new_p1);

  //---------------------------------------------------------------------------
  //: Return the length of `this'
  //---------------------------------------------------------------------------
  virtual double length() const; // pure virtual of vsol_curve_2d


  //---------------------------------------------------------------------------
  //: Get a dbgl_circ_arc
  //---------------------------------------------------------------------------
  dbgl_circ_arc dbgl_arc() const;


  //  ***************************************************************************
  //   Basic operations
  //  ***************************************************************************

  //  ---------------------------------------------------------------------------
  //  : Return the centre of the circle containing the arc
  //  ---------------------------------------------------------------------------
  vsol_point_2d_sptr arc_center() const;
  

  //  ---------------------------------------------------------------------------
  //  : Return the the curvature of the arc
  //  ---------------------------------------------------------------------------
  double curvature() const;

  //  ---------------------------------------------------------------------------
  //  : Is `p' in `this' ? (ie `p' verifies the equation, within some margin)
  //  ---------------------------------------------------------------------------
  virtual int in(vsol_point_2d_sptr const& p) const;

  //  ---------------------------------------------------------------------------
  //  : Returns the tangent to the circ_arc in the point p, if p is on the circ_arc.
  //    In general, returns the polar line of the point w.r.t. the circ_arc.
  //  ---------------------------------------------------------------------------
  virtual vgl_vector_2d<double> tangent(double s) const;

  //---------------------------------------------------------------------------
  //: Return the point on the arc boundary which is closest to the given point
  //---------------------------------------------------------------------------
  vsol_point_2d_sptr closest_point_on_curve(vsol_point_2d_sptr const& pt) const;

  //---------------------------------------------------------------------------
  //: Return the shortest distance of the point to the arc boundary
  //---------------------------------------------------------------------------
  double distance(vsol_point_2d_sptr const& query_pt) const;




  // ==== Binary IO methods ======


  //: Return a platform independent string identifying the class
  virtual vcl_string is_a() const { return "dbsol_circ_arc_2d"; }




};




#endif // dbsol_circ_arc_2d_h_
