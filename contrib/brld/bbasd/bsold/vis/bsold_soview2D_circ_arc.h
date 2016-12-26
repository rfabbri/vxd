// This is dbsol_soview2D_circ_arc.h
#ifndef dbsol_soview2D_circ_arc_h_
#define dbsol_soview2D_circ_arc_h_
//--------------------------------------------------------------------------------
//:
// \file
// \brief A vgui_soview2D circular arc object
// \author
//   Isabel Restrepo
//
// \verbatim
//  Modifications:
//   Isabel Restrepo    August 24, 2006  Initial version.
//  
// \endverbatim
//--------------------------------------------------------------------------------

#include <vcl_iosfwd.h>
#include <vsol/vsol_point_2d_sptr.h>
#include <vsol/vsol_point_2d.h>
#include <bgui/bgui_vsol_soview2D.h>
#include <dbsol/dbsol_circ_arc_2d_sptr.h>
#include <vsol/vsol_spatial_object_2d.h>
#include <dbsol/dbsol_circ_arc_2d.h>

//: dbsol_circ_arc_2d
// The current implementation is restricted to circ_arcs that are a real ellipse
class dbsol_soview2D_circ_arc : public bgui_vsol_soview2D
{
 public:
  //: Constructor - creates a view of a given dbsol_circ_arc_2d
  dbsol_soview2D_circ_arc( dbsol_circ_arc_2d_sptr const & circ_arc);

  //: Destructor - does nothing, smart pointers pass out of scope automatically
  ~dbsol_soview2D_circ_arc() {}

  //: Returns the type of this class ('bgui_vsol_soview2D_circ_arc').
  static vcl_string type_name_() { return "dbsol_soview2D_circ_arc"; }
  vcl_string type_name() const { return type_name_(); }

  //: Render this 2D digital_curve on the display.
  virtual void draw() const;

  // Returns a smart pointer to the arc
  // \note cast from a vsol_spatial_object_2d_sptr in the base class
  dbsol_circ_arc_2d_sptr sptr() const;

  //: Returns the distance squared from this curve to the given position.
  virtual float distance_squared(float x, float y) const;

  //: Returns the centroid of the circ_arc
  void get_centroid(float* x, float* y) const;

  //: Translate this circ_arc  by the given x and y distances.
  virtual void translate(float x, float y);

 protected:
   protected:
   //: the center of the arc
   vsol_point_2d c;
    //: radius of the arc
   double r;
    //: length of the arc
   double arclen;
   //: a list of arc points
   vcl_vector< vsol_point_2d_sptr > arc_points;
};

#endif // dbgui_vsol_soview2D_h_
